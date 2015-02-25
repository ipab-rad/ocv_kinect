#include <XnCppWrapper.h>

#define SAMPLE_PATH "../SamplesConfig.xml"


xn::Context context;
xn::ScriptNode scriptNode;


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

    printf("So far so good ...");

}