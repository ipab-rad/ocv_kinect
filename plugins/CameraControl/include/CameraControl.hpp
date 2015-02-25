#ifndef _CAMERA_CONTROL_H
#define _CAMERA_CONTROL_H

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

#include <gazebo/math/gzmath.hh>

#include <gesture.hpp>
#include <vec3.hpp>


namespace gazebo {

    class CameraControl: public SystemPlugin {
        
        public:
            CameraControl();
            ~CameraControl();
            void Load(int argc, char** argv);
        
        private:
            bool initialized;
            event::ConnectionPtr updateConnection;
            rendering::UserCameraPtr cam;
            math::Vector3 inputVector;

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
    };

}
#endif