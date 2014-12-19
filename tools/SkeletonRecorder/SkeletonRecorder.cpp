#include <iostream>
#include <fstream>
#include <XnCppWrapper.h>

#define SAMPLE_PATH "SamplesConfig.xml"
#define MAX_USERS 16


xn::Context context;
xn::ScriptNode scriptNode;
xn::DepthGenerator depthGenerator;
xn::UserGenerator userGenerator;


void XN_CALLBACK_TYPE OnNewUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie) {
    printf("New user.\n");
    userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

void XN_CALLBACK_TYPE OnLostUser(xn::UserGenerator& gen, XnUserID nId, void* pCookie) {
    printf("Lost user.\n");
}

void XN_CALLBACK_TYPE OnPoseDetected(xn::PoseDetectionCapability& cap, const XnChar* strPose, XnUserID nId, void* pCookie) {
    printf("Pose detected.\n");
}

void XN_CALLBACK_TYPE OnCalibrationStart(xn::SkeletonCapability& cap, XnUserID nId, void* pCookie) {
    printf("Calibration start.\n");
}

void XN_CALLBACK_TYPE OnCalibrationComplete(xn::SkeletonCapability& cap, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie) {
    if (eStatus == XN_CALIBRATION_STATUS_OK) {
        printf("Calibration end.\n");
        userGenerator.GetSkeletonCap().StartTracking(nId);
    }
}

void WriteSkeletonState(std::ofstream& outfile, xn::SkeletonCapability& skelly, XnUserID nId) {
    XnSkeletonJoint activeJoints[24];
    XnUInt16 size = 24;
    skelly.EnumerateActiveJoints(activeJoints, size);
    XnSkeletonJointTransformation transform;
    for (int i=0; i<size; i++) {
        skelly.GetSkeletonJoint(nId, activeJoints[i], transform);
        outfile << activeJoints[i] << ' ';
        XnVector3D pos = transform.position.position;
        outfile << pos.X << ' ' << pos.Y << ' ' << pos.Z << ' ';
        for (int i=0; i<9; i++) {
            outfile << transform.orientation.orientation.elements[i] << ' ';
        }
        outfile << '\n';
    }
    outfile << '\n';
}

int main() {
    XnBool fileExists;
    xnOSDoesFileExist(SAMPLE_PATH, &fileExists);
    if (!fileExists) {
        printf("Could not find config file: %s. Aborting.\n", SAMPLE_PATH);
        return XN_STATUS_ERROR;
    }

    xn::EnumerationErrors errors;
    XnStatus status = context.InitFromXmlFile(
            SAMPLE_PATH, scriptNode, &errors);
    if (status == XN_STATUS_NO_NODE_PRESENT) {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return status;
    } else if (status != XN_STATUS_OK) {
        printf("Open failed: %s\n", xnGetStatusString(status));
        return status;
    }

    status = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depthGenerator);
    printf("So far so good ...\n");

    printf("Creating UserGenerator.\n");
    status = context.FindExistingNode(XN_NODE_TYPE_USER, userGenerator);
    if (status != XN_STATUS_OK) {
        status = userGenerator.Create(context);
    }
    XnCallbackHandle userCallbacks, calibrationStart, calibrationComplete, poseDetected;
    status = userGenerator.RegisterUserCallbacks(OnNewUser, OnLostUser, NULL, userCallbacks);
    status = userGenerator.GetSkeletonCap().RegisterToCalibrationStart(OnCalibrationStart, NULL, calibrationStart);
    status = userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(OnCalibrationComplete, NULL, calibrationComplete);

    if (userGenerator.GetSkeletonCap().NeedPoseForCalibration()) {
        printf("Need pose!!!");
    }

    userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    XnUserID users[MAX_USERS];

    // Prepare file for writing to.
    std::ofstream outfile;
    outfile.open("skeleton_recording");

    context.StartGeneratingAll();
    while (!xnOSWasKeyboardHit()) {
        context.WaitOneUpdateAll(userGenerator);
        int tracking = 0;
        XnUInt16 maxUsers = MAX_USERS;
        userGenerator.GetUsers(users, maxUsers);
        for (XnUInt16 i=0; i<maxUsers; i++) {
            xn::SkeletonCapability skelly = userGenerator.GetSkeletonCap();
            if (skelly.IsTracking(users[i]) == true) {
                tracking += 1;
            }
            if (i == 0) {
                WriteSkeletonState(outfile, skelly, users[0]);
            }
        }
        //printf("Tracking %d users.\n", tracking);
    }

    outfile.close();

    userGenerator.Release();
    depthGenerator.Release();
    scriptNode.Release();
    context.Release();
}