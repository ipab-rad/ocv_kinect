#include <iostream>
#include <GestureServer.hpp>

#define SAMPLE_PATH "SamplesConfig.xml"
#define MAX_USERS 16


GestureServer::GestureServer() {
    this->initialized = false;
}

GestureServer::~GestureServer() {
    if (this->initialized) {
        this->userGenerator.Release();
        this->depthGenerator.Release();
        this->scriptNode.Release();
        this->context.Release();
    }
}

int GestureServer::Initialize() {
    if (!this->ConfigFileExists()) {
        printf("Could not find config file.\n");
        return XN_STATUS_ERROR;
    }
    XnStatus status = this->InitializeContext();
    if (status != XN_STATUS_OK) return status;

    this->initialized = true;
}

bool GestureServer::ConfigFileExists() {
    XnBool fileExists;
    xnOSDoesFileExist(SAMPLE_PATH, &fileExists);
    return fileExists;
}

XnStatus GestureServer::InitializeContext() {
    xn::EnumerationErrors errors;
    XnStatus status = context.InitFromXmlFile(
        SAMPLE_PATH, scriptNode, &errors);
    if (status == XN_STATUS_NO_NODE_PRESENT) {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
    } else if (status != XN_STATUS_OK) {
        printf("Open failed: %s\n", xnGetStatusString(status));
    }
    return status;
}

int main() {
    GestureServer* gs = new GestureServer();
    gs->Initialize();
    delete gs;
}