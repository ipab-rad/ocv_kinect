#include <gazebo/gazebo.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/math/gzmath.hh>

#include <CameraControl.hpp>

#define SERVER_IP "127.0.0.1"
#define PORT "6360"


namespace gazebo {

    CameraControl::CameraControl() {
        this->inputVector = math::Vector3(1.0, 0.0, 0.0);
    }

    CameraControl::~CameraControl() {
        event::Events::DisconnectPreRender(this->updateConnection);
        this->cam.reset();
    }

    void CameraControl::Load(int argc, char** argv) {
        printf("Loading plugin...\n");
        this->initialized = false;
        this->updateConnection = event::Events::ConnectPreRender(
                boost::bind(&CameraControl::Update, this));
    }

    void CameraControl::Init() {

    }

    void CameraControl::Setup() {
        printf("Initializing... ");
        this->cam = gui::get_active_camera();
        this->InitializeSocket();
        this->initialized = true;
        this->buffer_size = gesture_size();
        printf("Initialized.");
    }

    void CameraControl::InitializeSocket() {
        struct
    }

    void CameraControl::Update() {
        if (!initialized) this->Setup();
        math::Vector3 pos = this->cam->GetWorldPosition();
        pos += this->inputVector;
        this->cam->SetWorldPosition(pos);
    }

    GZ_REGISTER_SYSTEM_PLUGIN(CameraControl)

}