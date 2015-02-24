#ifndef _CAMERA_CONTROL_H
#define _CAMERA_CONTROL_H


#include <gazebo/math/gzmath.hh>

namespace gazebo {

    class CameraControl: public SystemPlugin {
        
        public:
            CameraControl();
            ~CameraControl();
            void Load(int argc, char** argv);
        
        private:
            bool initialized;
            event::ConnectionPtr updateConnection;
            rendering::UserCameraPtr cam;
            math::Vector3 inputVector;

            void Init();
            void Setup();
            void Update();
    };

}
#endif