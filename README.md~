OCV Kinect
==========

This comprises two main programs:

GestureServer
-------------
A program which uses the Kinect to detect user gestures and send the information over a socket to another machine.

OccamControl
------------
A Gazebo plugin that receives gesture information and moves the viewpoint of a user wearing an Oculus Rift within a Gazebo simulation.

Requirements
============
OCV Kinect requires Scons, and Gazebo 4 to compile and run.

Additionally, Gazebo 4 must be compiled with Oculus Rift support. Instructions for doing that are here:

[http://www.gazebosim.org/tutorials?tut=oculus&cat=rendering][]

Reading this will help explain how Gazebo works with the Oculus Rift in the first place.

Anyway, your gui.ini file should look like this:

```
[geometry]
x=0
y=0

[oculus]
x={Monitor width in pixels}
y=0
visual=occam::link::visual
autolaunch=0
```
The 'occam' model is used in all of the Worlds in this project. You can use a different model name.

Compiling
=========
Simply run the command `scons` from the project root directory.

Usage
=====
GestureServer
-------------

To run the GestureServer program:
```
$ cd ./tools/GestureServer
$ ./bin/GestureServer IP_ADDRESS PORT
```

The IP_ADDRESS argument must be the IP address of the machine running Gazebo. The port can be anything, as long as it matches the port number used by OccamControl.


OccamControl
------------
(Note: you will have to modify ./plugins/OccamControl/src/OccamControl.cpp and change the definitions of CLIENT_IP and PORT at the top of the file. There are a hundred better ways to do this, but I never got around to fixing it. Once they're correct, re-compile the project.)

Firstly, you must have a Gazebo World that contains a model named 'occam' (or whatever your gui.ini says). That model must contain a visual link (according to Gazebo, I haven't tried without one.) That model must also contain the OccamControl plugin:

    `<plugin name="OccamControl" filename="./plugins/OccamControl/bin/libOccamControl.so"/>`

Once that's all in order, you'll have to make sure that OpenNI is on the library path:

    `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"./external/OpenNI/Lib"`

(Note that both of these paths may be different if you're running Gazebo from a directory other than this project's root directory.)

Finally, run Gazebo:

    `gazebo world-name.world`

From the menu, select Window -> Oculus Rift. If the Oculus Rift is set up correctly, you should now see the simulation from the viewpoint of the 'occam' model in the scene.

Using the Kinect
----------------
If you have a machine running your simulation in Gazebo, with the OccamControl plugin on the correct model, setup the Kinect on another machine (or optionally the same one - but for various reasons the dependencies did not play nicely together for me).

On the machine with the Kinect run the GestureServer, choose Window -> Oculus Rift in Gazebo on the other machine, wear the headset, and stand in front of the Kinect.

Hold both your hands at shoulder level in front of you. From there, move your right hand in any direction to move in that direction, and move your left hand left-to-right in order to rotate on the spot.

It admittedly takes some getting used to, but once you do it is fairly easy to move around. (Unless you get motion sick, and then what you need is a [Virtual Nose](http://www.purdue.edu/newsroom/releases/2015/Q1/virtual-nose-may-reduce-simulator-sickness-in-video-games.html))