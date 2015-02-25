loc = {
    "openni_inc":   ["#external/OpenNI/Include"],
    "openni_lib":   ["#external/OpenNI/Lib"],
    "tpl_inc":      ["#external/tpl"],
    "tpl_src":      ["#external/tpl/tpl.c"],
    "util_inc":     ["#utils/include"],
    "util_src":     Glob("#utils/src/*.cpp")
}

env = Environment()

SConscript([
    "tools/GestureServer/SConscript",
    "plugins/CameraControl/SConscript"
    ], exports=["loc", "env"])