#include <gazebo/gazebo.hh>
#include <gazebo/math/gzmath.hh>
#include "gazebo/physics/physics.hh"
#include "keyboardcontroller.hpp"


namespace gazebo {

	KeyboardController::KeyboardController() {

	}

	KeyboardController::~KeyboardController() {
		event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
	}

	void KeyboardController::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) {
        printf("Loading plugin\n");
        this->updateConnection = event::Events::ConnectPreRender(
            boost::bind(&KeyboardController::Update, this));

        this->node = transport::NodePtr(new transport::Node());
  		this->node->Init(_world->GetName());
        this->publisher = this->node->Advertise<msgs::Vector3d>("~/camcontrol");
    }

    void KeyboardController::Init() {
    }
            
    void KeyboardController::Update() {
        printf("Keyboard plugin update\n");
		math::Vector3 direction = math::Vector3(1.0, 0.0, 0.0);
        msgs::Vector3d msg = msgs::Convert(direction);
        this->publisher->Publish(msg);
    }

	GZ_REGISTER_WORLD_PLUGIN(KeyboardController)

}