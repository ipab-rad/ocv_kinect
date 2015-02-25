#ifndef _CAMERA_CONTROL_H
#define _CAMERA_CONTROL_H


#include <gazebo/math/gzmath.hh>

namespace gazebo {

	typedef const boost::shared_ptr<const msgs::Vector3d> ConstVector3dPtr;

	class CameraControlPlugin: public SystemPlugin {
        
        public:
            CameraControlPlugin();
            ~CameraControlPlugin();
            void Load(int argc, char** argv);
        
        private:
            bool initialized;
    		event::ConnectionPtr updateConnection;
        	rendering::UserCameraPtr cam;
        	transport::NodePtr node;
        	transport::SubscriberPtr subscriber;
        	math::Vector3 inputVector;

            void Init();
            void Initialize();
            void Update();
            void OnInput(ConstVector3dPtr &_msg);
            math::Vector3 getOffsetVector(double magnitude);
    };

}
#endif