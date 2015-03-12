#ifndef _OCCAM_CONTROL_H
#define _OCCAM_CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/math/gzmath.hh>

#include <gesture.hpp>
#include <vec3.hpp>


namespace gazebo {

    class OccamControl: public ModelPlugin {
        
        public:
            OccamControl();
            ~OccamControl();
            void Load(physics::ModelPtr occammodel, sdf::ElementPtr _sdf);
            void OnUpdate(const common::UpdateInfo &_info);
        
        private:
            bool initialized;
            physics::ModelPtr model;
            event::ConnectionPtr updateConnection;

            struct addrinfo* hosts;
            struct addrinfo* host;
            int sock;
            int buffer_size;
            struct sockaddr_storage server_addr;
            socklen_t addr_len;

            void Init();
            int InitializeSocket();
            void Setup();
            void Update();
            gesture ReceiveGesture();
            void MoveCamera(gesture g);
    };

}
#endif