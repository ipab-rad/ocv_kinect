#include <iostream>
#include <algorithm>
#include <cmath>
#include <GestureServer.hpp>


#define SAMPLE_PATH "SamplesConfig.xml"
#define MAX_USERS 16

#define INPUTEXTENT 150.0
#define DEADZONE 0.4
#define LOGGING_POSITION true

vec3 NEUTRAL_OFFSET = {75.0, 0, -150.0};


GestureServer::GestureServer() {
    this->initialized = false;
}

GestureServer::~GestureServer() {
    if (this->initialized) {
        this->userGenerator.Release();
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

    status = this->InitializeUserGenerator();
    if (status != XN_STATUS_OK) return status;

    this->InitializeCallbacks();

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

    printf("Beginning gesture tracking:");
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
    skelly.GetSkeletonJointPosition(user, XN_SKEL_RIGHT_SHOULDER, rightShoulderPos);
    skelly.GetSkeletonJointPosition(user, XN_SKEL_RIGHT_HAND, rightHandPos);
    vec3 hand;
    vec3 shoulder;
    hand.set(rightHandPos.position);
    shoulder.set(rightShoulderPos.position);
    vec3 movementVector = this->CalculateMovementVector(hand, shoulder);
    if (LOGGING_POSITION) {
        printf("Input: %.3f %.3f %.3f\n", movementVector.x, movementVector.y, movementVector.z);
    }

}

vec3 GestureServer::CalculateMovementVector(const vec3& hand, const vec3& shoulder) {
    vec3 neutralPos = shoulder + NEUTRAL_OFFSET;
    vec3 direction = (hand - neutralPos) / INPUTEXTENT;
    vec3 movement = direction.squash(DEADZONE).shrink(DEADZONE) / (1-DEADZONE);
    return movement;
}


int main() {
    GestureServer* gs = new GestureServer();
    gs->Initialize();
    gs->StartTrackingGestures();
    delete gs;
}