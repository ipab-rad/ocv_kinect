#ifndef _KEYBOARD_CONTROLLER_H
#define _KEYBOARD_CONTROLLER_H


namespace gazebo {

	class KeyboardController: public WorldPlugin {
        
        public:
            KeyboardController();
            ~KeyboardController();
            void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);
        
        private:
            void Init();
            void Update();
            event::ConnectionPtr updateConnection;
            transport::PublisherPtr publisher;
            transport::NodePtr node;
    };

}

#endif