#include <iostream>
#include <algorithm>
#include <cmath>
#include <GestureServer.hpp>


#define SAMPLE_PATH "SamplesConfig.xml"
#define MAX_USERS 16

#define INPUTEXTENT 150.0
#define DEADZONE 0.4
#define LOGGING_POSITION false

vec3 NEUTRAL_OFFSET_RIGHT = {75.0, 0, -150.0};
vec3 NEUTRAL_OFFSET_LEFT = {-75.0, 0, -150.0};


GestureServer::GestureServer(const char* client_ip, const char* port) {
    this->initialized = false;
    this->client_ip = client_ip;
    this->port = port;
}

GestureServer::~GestureServer() {
    if (this->initialized) {
        this->userGenerator.Release();
        this->scriptNode.Release();
        this->context.Release();
        freeaddrinfo(this->hosts);
        close(this->sock);
    }
}

int GestureServer::Initialize() {
    if (!this->ConfigFileExists()) {
        printf("Could not find config file.\n");
        return XN_STATUS_ERROR;
    }
    XnStatus status = this->InitializeContext();
    if (status != XN_STATUS_OK) return status;

    status = this->InitializeUserGenerator();
    if (status != XN_STATUS_OK) return status;

    this->InitializeCallbacks();

    int sockstatus = this->InitializeSocket();
    if (sockstatus != 0) {
        printf("Failed to initialize socket.\n");
        return sockstatus;
    }

    this->buffer_size = gesture_size();

    this->initialized = true;
}

bool GestureServer::ConfigFileExists() {
    XnBool fileExists;
    xnOSDoesFileExist(SAMPLE_PATH, &fileExists);
    return fileExists;
}

XnStatus GestureServer::InitializeContext() {
    printf("Initializing context.\n");
    xn::EnumerationErrors errors;
    XnStatus status = this->context.InitFromXmlFile(
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

XnStatus GestureServer::InitializeUserGenerator() {
    printf("Initializing user generator.\n");
    XnStatus status;
    status = this->context.FindExistingNode(XN_NODE_TYPE_USER, this->userGenerator);
    if (status != XN_STATUS_OK) {
        status = this->userGenerator.Create(this->context);
    }
    return status;
}

XnStatus GestureServer::InitializeCallbacks() {
    printf("Initializing callbacks.\n");
    XnCallbackHandle userCallbacks, calibStartCallback, calibCompleteCallback, poseDetectedCallback;
    this->userGenerator.RegisterUserCallbacks(OnNewUser, OnLostUser, NULL, userCallbacks);
    this->userGenerator.GetSkeletonCap().RegisterToCalibrationStart(OnCalibStart, NULL, calibStartCallback);
    this->userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(OnCalibComplete, this, calibCompleteCallback);
    if (this->userGenerator.GetSkeletonCap().NeedPoseForCalibration()) {
        printf("You need a calibration pose!!! Looks like you did something wrong, go fix the bug.\n");
    }
    this->userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
}

int GestureServer::InitializeSocket() {
    int status;
    struct addrinfo hints;
    struct addrinfo* host;
    bool socket_bound = false;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(this->client_ip, this->port, &hints, &hosts);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for(host=hosts; host!=NULL; host=host->ai_next) {
        sock = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
        if (sock == -1) continue;

        socket_bound = true;
        this->host = host;
        this->sock = sock;
        break;
    }

    if (!socket_bound) {
        printf("Error getting socket.\n");
        return 2;
    }
}

void XN_CALLBACK_TYPE GestureServer::OnNewUser(xn::UserGenerator& gen, XnUserID user, void* pCookie) {
    printf("New user found.\n");
    gen.GetSkeletonCap().RequestCalibration(user, true);
}

void XN_CALLBACK_TYPE GestureServer::OnLostUser(xn::UserGenerator& gen, XnUserID user, void* pCookie) {
    printf("User lost.\n");
}

void XN_CALLBACK_TYPE GestureServer::OnCalibStart(xn::SkeletonCapability& cap, XnUserID user, void* pCookie) {
    printf("Calibration started.\n");
}

void XN_CALLBACK_TYPE GestureServer::OnCalibComplete(xn::SkeletonCapability& cap, XnUserID user, XnCalibrationStatus eStatus, void* pCookie) {
    if (eStatus == XN_CALIBRATION_STATUS_OK) {
        printf("Calibration complete.\n");
        GestureServer* server = static_cast<GestureServer*>(pCookie);
        server->StartTracking(user);
    }
}

void GestureServer::StartTracking(XnUserID user) {
    this->userGenerator.GetSkeletonCap().StartTracking(user);
}

void GestureServer::StartTrackingGestures() {
    XnUserID users[MAX_USERS];

    printf("Beginning gesture tracking:\n");
    this->context.StartGeneratingAll();
    while (!xnOSWasKeyboardHit()) {
        this->context.WaitOneUpdateAll(this->userGenerator);
        int tracking = 0;
        XnUInt16 maxUsers = MAX_USERS;
        this->userGenerator.GetUsers(users, maxUsers);
        for (XnUInt16 i=0; i<maxUsers; i++) {
            xn::SkeletonCapability skelly = this->userGenerator.GetSkeletonCap();
            if (skelly.IsTracking(users[i]) == true) {
                tracking += 1;
                this->SendGesture(skelly, users[i]);
            }
        }
    }
}

void GestureServer::SendGesture(xn::SkeletonCapability& skelly, XnUserID user) {
    XnSkeletonJointPosition rightShoulderPos;
    XnSkeletonJointPosition rightHandPos;
    XnSkeletonJointPosition leftShoulderPos;
    XnSkeletonJointPosition leftHandPos;
    skelly.GetSkeletonJointPosition(user, XN_SKEL_RIGHT_SHOULDER, rightShoulderPos);
    skelly.GetSkeletonJointPosition(user, XN_SKEL_RIGHT_HAND, rightHandPos);
    skelly.GetSkeletonJointPosition(user, XN_SKEL_LEFT_SHOULDER, leftShoulderPos);
    skelly.GetSkeletonJointPosition(user, XN_SKEL_LEFT_HAND, leftHandPos);
    vec3 rhand;
    vec3 rshoulder;
    rhand.set(rightHandPos.position);
    rshoulder.set(rightShoulderPos.position);
    vec3 lhand;
    vec3 lshoulder;
    lhand.set(leftHandPos.position);
    lshoulder.set(leftShoulderPos.position);
    vec3 movementVector = this->CalculateMovementVector(rhand, rshoulder);
    double rotation = this->CalculateRotation(lhand, lshoulder);
    if (LOGGING_POSITION) {
        printf("Input: %.3f %.3f %.3f\n", movementVector.x, movementVector.y, movementVector.z);
    }
    gesture g;
    g.movement = movementVector;
    g.rotation = rotation;
    this->SendData(g);
}

vec3 GestureServer::CalculateMovementVector(const vec3& hand, const vec3& shoulder) {
    vec3 neutralPos = shoulder + NEUTRAL_OFFSET_RIGHT;
    vec3 direction = (hand - neutralPos) / INPUTEXTENT;
    vec3 movement = direction.squash(DEADZONE).shrink(DEADZONE) / (1-DEADZONE);
    return movement.clampcomponents(-1.0, 1.0);
}

double GestureServer::CalculateRotation(const vec3& hand, const vec3& shoulder) {
    vec3 neutralPos = shoulder + NEUTRAL_OFFSET_LEFT;
    vec3 direction = (hand - neutralPos) / INPUTEXTENT;
    vec3 movement = direction.squash(DEADZONE).shrink(DEADZONE) / (1-DEADZONE);
    return movement.clampcomponents(-1.0, 1.0).x;
}

void GestureServer::SendData(const gesture& gesture) {
    char buffer[this->buffer_size];
    serialize_gesture(gesture, buffer, this->buffer_size);
    int sentbytes = sendto(this->sock, buffer, this->buffer_size, 0,
        this->host->ai_addr, this->host->ai_addrlen);
    if (sentbytes == -1) {
        printf("Error sending data.\n");
    }
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: GestureServer client_ip port\n");
        exit(1);
    }
    GestureServer* gs = new GestureServer(argv[1], argv[2]);
    gs->Initialize();
    gs->StartTrackingGestures();
    delete gs;
}