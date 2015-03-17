#ifndef _GESTURESERVER_H
#define _GESTURESERVER_H

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

#include <XnCppWrapper.h>
#include <vec3.hpp>
#include <gesture.hpp>


class GestureServer {

    public:
        GestureServer(const char* client_ip, const char* port);
        ~GestureServer();
        int Initialize();
        void StartTracking(XnUserID user);
        void StartTrackingGestures();
        
    private:
        bool initialized;
        xn::Context context;
        xn::ScriptNode scriptNode;
        xn::UserGenerator userGenerator;
        const char* client_ip;
        const char* port;
        struct addrinfo* hosts;
        struct addrinfo* host;
        int sock;
        int buffer_size;

        bool ConfigFileExists();
        XnStatus InitializeContext();
        XnStatus InitializeUserGenerator();
        XnStatus InitializeCallbacks();
        int InitializeSocket();
        void SendGesture(xn::SkeletonCapability& skelly, XnUserID user);
        void SendData(const gesture& gesture);
        vec3 CalculateMovementVector(const vec3& hand, const vec3& shoulder);
        double CalculateRotation(const vec3& hand, const vec3& shoulder);

        static void XN_CALLBACK_TYPE OnNewUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnLostUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnCalibStart(xn::SkeletonCapability& cap, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnCalibComplete(xn::SkeletonCapability& cap, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
};

#endif