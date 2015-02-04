#ifndef _GESTURESERVER_H
#define _GESTURESERVER_H

#include <XnCppWrapper.h>


class GestureServer {

    public:
        GestureServer();
        ~GestureServer();
        int Initialize();
        XnStatus InitializeContext();
        
    private:
        bool initialized;
        xn::Context context;
        xn::ScriptNode scriptNode;
        xn::DepthGenerator depthGenerator;
        xn::UserGenerator userGenerator;

        bool ConfigFileExists();
};

#endif