#include <gazebo/gazebo.hh>
#include <gazebo/gui/GuiIface.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/math/gzmath.hh>

namespace gazebo {

    class KinectControlPlugin: public SystemPlugin {

        std::vector<event::ConnectionPtr> connections;
        rendering::UserCameraPtr cam;
        math::Vector3 pos;
        
        public:
            ~KinectControlPlugin();
            void Load(int argc, char** argv);
        
        private:
            void Init();
            void Update();
    };
        
        
        
    KinectControlPlugin::~KinectControlPlugin() {
        this->connections.clear();
        this->cam.reset();
    }
            
    void KinectControlPlugin::Load(int argc, char** argv) {
        this->connections.push_back(
            event::Events::ConnectPreRender(
                boost::bind(&KinectControlPlugin::Update, this)));
    }
            
    void KinectControlPlugin::Init() {
    }
            
    void KinectControlPlugin::Update() {
        if (!this->cam) {
            this->cam = gui::get_active_camera();
            this->pos = this->cam->GetWorldPosition();
        }
        //double x = pos.x;
        //double y = pos.y;
        //double z = pos.z;
        //pos.Set(x+0.1, y, z);
        this->cam->SetWorldPosition(pos);
    }

    GZ_REGISTER_SYSTEM_PLUGIN(KinectControlPlugin)

}
