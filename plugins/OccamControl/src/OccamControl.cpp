#include <boost/bind.hpp>
#include <stdio.h>

#include <OccamControl.hpp>
#include <unistd.h>
#include <fcntl.h>

#define CLIENT_IP "129.215.96.52"
#define PORT "6360"
#define MOVESPEED 0.1
#define TURNSPEED 0.025
#define DUMMIED false

namespace gazebo {

    OccamControl::OccamControl() {

    }

    OccamControl::~OccamControl() {
        if (initialized) {
            freeaddrinfo(this->hosts);
            close(this->sock);
        }
        event::Events::DisconnectPreRender(this->updateConnection);
    }

    void OccamControl::Load(physics::ModelPtr occammodel, sdf::ElementPtr _sdf) {
        printf("Loading OccamControl...\n");
        this->initialized = false;
        this->model = occammodel;
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&OccamControl::OnUpdate, this, _1));
    }

    void OccamControl::OnUpdate(const common::UpdateInfo & _info) {
        if (!initialized) {
            this->Setup();
        } else if (!DUMMIED) {
            this->ReceiveGesture();
        }
    }

    void OccamControl::MoveCamera(gesture g) {
        g.movement = g.movement * MOVESPEED;
        g.rotation = g.rotation * TURNSPEED;
        math::Pose pose = this->model->GetWorldPose();
        math::Quaternion turn(0, 0, -g.rotation);
        math::Quaternion rot = pose.rot * turn;
        math::Vector3 mov(-g.movement.z, -g.movement.x, g.movement.y);
        math::Vector3 pos = pose.pos + (rot * mov);
        pose.pos = this->BoundPosition(pos);
        pose.rot = rot;
        this->model->SetWorldPose(pose);
    }

    math::Vector3 OccamControl::BoundPosition(math::Vector3 pos) {
        double bounds = 20.0;
        double x = pos.x;
        double y = pos.y;
        double z = pos.z;
        if (x < -bounds) x = -bounds;
        if (x > bounds) x = bounds;
        if (y < -bounds) y = -bounds;
        if (y > bounds) y = bounds;
        if (z < 1.0) z = 1.0;
        if (z > bounds) z = bounds;
        return math::Vector3(x, y, z);
    }

    void OccamControl::Setup() {
        printf("Initializing... ");
        this->buffer_size = gesture_size();
        this->initialized = (this->InitializeSocket() == 0);
        if (this->initialized) {
            printf("Initialized.\n");
        } else {
            printf("Error initializing socket.\n");
            exit(1);
        }
    }

    void OccamControl::Init() {

    }

    int OccamControl::InitializeSocket() {
        int status;
        struct addrinfo hints;
        struct addrinfo* host;
        bool socket_bound = false;
        this->addr_len = sizeof this->server_addr;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        status = getaddrinfo(CLIENT_IP, PORT, &hints, &hosts);
        if (status != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 1;
        }

        for(host=this->hosts; host!=NULL; host=host->ai_next) {
            sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
            fcntl(sock, F_SETFL, O_NONBLOCK);
            if (sock == -1) continue;
            int bindstatus = bind(sock, host->ai_addr, host->ai_addrlen);
            if (bindstatus == -1) {
                close(sock);
                continue;
            }
            socket_bound = true;
            this->host = host;
            this->sock = sock;
            break;
        }

        if (!socket_bound) {
            printf("Error binding socket.\n");
            return 2;
        }
        return 0;
    }

    void OccamControl::ReceiveGesture() {
        char buffer[this->buffer_size];
        int recdbytes = recvfrom(this->sock, buffer, this->buffer_size, 0,
            (struct sockaddr*) &server_addr, &addr_len);
        gesture g;
        if (recdbytes > 0) {
            g = deserialize_gesture(buffer, buffer_size);
            this->MoveCamera(g);
        }
    }

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(OccamControl)

};