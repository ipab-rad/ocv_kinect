#ifndef _GESTURESERVER_H
#define _GESTURESERVER_H

#include <XnCppWrapper.h>
#include <vec3.hpp>


class GestureServer {

    public:
        GestureServer();
        ~GestureServer();
        int Initialize();
        void StartTrackingGestures();
        
    private:
        bool initialized;
        xn::Context context;
        xn::ScriptNode scriptNode;
        xn::UserGenerator userGenerator;

        bool ConfigFileExists();
        XnStatus InitializeContext();
        XnStatus InitializeUserGenerator();
        XnStatus InitializeCallbacks();
        void SendGesture(xn::SkeletonCapability& skelly, XnUserID user);
        vec3 CalculateMovementVector(const vec3& hand, const vec3& shoulder);

        static void XN_CALLBACK_TYPE OnNewUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnLostUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnCalibStart(xn::SkeletonCapability& cap, XnUserID nId, void* pCookie);
        static void XN_CALLBACK_TYPE OnCalibComplete(xn::SkeletonCapability& cap, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
};

#endif