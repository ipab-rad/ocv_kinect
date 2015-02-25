#include <gazebo/gazebo.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>

#include <CameraControl.hpp>

#define SERVER_IP "127.0.0.1"
#define PORT "6360"


namespace gazebo {

    CameraControl::CameraControl() {
        this->inputVector = math::Vector3(1.0, 0.0, 0.0);
    }

    CameraControl::~CameraControl() {
        if (initialized) {
            freeaddrinfo(this->hosts);
            close(this->sock);
        }
        event::Events::DisconnectPreRender(this->updateConnection);
        this->cam.reset();
    }

    void CameraControl::Load(int argc, char** argv) {
        printf("Loading plugin...\n");
        this->initialized = false;
        this->updateConnection = event::Events::ConnectPreRender(
                boost::bind(&CameraControl::Update, this));
    }

    void CameraControl::Init() {

    }

    void CameraControl::Setup() {
        printf("Initializing... ");
        this->cam = gui::get_active_camera();
        this->buffer_size = gesture_size();
        this->initialized = (this->InitializeSocket() == 0);
        if (this->initialized) {
            printf("Initialized.\n");
        } else {
            printf("Error initializing socket.\n");
            exit(1);
        }
    }

    int CameraControl::InitializeSocket() {
        int status;
        struct addrinfo hints;
        struct addrinfo* host;
        bool socket_bound = false;
        this->addr_len = sizeof this->server_addr;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        status = getaddrinfo(SERVER_IP, PORT, &hints, &hosts);
        if (status != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 1;
        }

        for(host=this->hosts; host!=NULL; host=host->ai_next) {
            sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
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

    void CameraControl::Update() {
        if (!initialized) this->Setup();
        struct gesture g = this->ReceiveGesture();

        math::Vector3 pos = this->cam->GetWorldPosition();
        pos.Set(
                pos.x + g.movement.x,
                pos.y + g.movement.y,
                pos.z + g.movement.z
            );
        this->cam->SetWorldPosition(pos);
    }

    gesture CameraControl::ReceiveGesture() {
        char buffer[this->buffer_size];
        int recdbytes = recvfrom(this->sock, buffer, this->buffer_size, 0,
            (struct sockaddr*) &server_addr, &addr_len);
        gesture g;
        if (recdbytes == -1) {
            printf("Error receiving bytes.\n");
        } else {
            printf("Received %d bytes: ", recdbytes);
            printf("%s", buffer);
            g = deserialize_gesture(buffer, buffer_size);
        }
        return g;
    }

    GZ_REGISTER_SYSTEM_PLUGIN(CameraControl)

}