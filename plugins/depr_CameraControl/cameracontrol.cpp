#include <gazebo/gazebo.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/math/gzmath.hh>
#include <math.h>

#include "cameracontrol.hpp"


namespace gazebo {
        
    CameraControlPlugin::CameraControlPlugin() {
        this->inputVector = math::Vector3(0.0, 0.0, 0.0);
    }
    
    CameraControlPlugin::~CameraControlPlugin() {
        event::Events::DisconnectPreRender(this->updateConnection);
        this->cam.reset();
    }
            
    void CameraControlPlugin::Load(int argc, char** argv) {
        printf("Loading plugin\n");
        this->initialized = false;
        this->updateConnection = event::Events::ConnectPreRender(
                boost::bind(&CameraControlPlugin::Update, this));
    }
            
    void CameraControlPlugin::Init() {
        
    }

    void CameraControlPlugin::Initialize() {
        printf("Initializing... ");
        this->node = transport::NodePtr(new transport::Node());
        this->node->Init(gui::get_world());
        this->subscriber = this->node->Subscribe("~/camcontrol",
            &CameraControlPlugin::OnInput, this);
        this->cam = gui::get_active_camera();
        this->initialized = true;
        printf("Initialized.\n");
    }

    void CameraControlPlugin::OnInput(ConstVector3dPtr &_msg) {
        double x = _msg->x();
        double y = _msg->y();
        double z = _msg->z();
        this->inputVector += math::Vector3(x, y, z);
    }
            
    void CameraControlPlugin::Update() {
        if (!initialized) {
            this->Initialize();
        }
        math::Vector3 pos = this->cam->GetWorldPosition();
        pos += this->inputVector;
        this->cam->SetWorldPosition(pos);
        inputVector.Set(0.0, 0.0, 0.0);
    }

    math::Vector3 CameraControlPlugin::getOffsetVector(double magnitude) {
        math::Pose pose = this->cam->GetWorldPose();
        math::Vector3 side = pose.rot * math::Vector3::UnitY;
        return side * magnitude;
    }

    GZ_REGISTER_SYSTEM_PLUGIN(CameraControlPlugin)

}
