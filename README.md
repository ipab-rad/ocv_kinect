ocv_kinect
==========

Oculus Rift + Kinect POV control

Running the project
===================

On a monitor
------------
First, set the plugin path for Gazebo by sourcing the `gazebopluginpath` file.

	`$ source gazebopluginpath`

Then start the Gazebo server:

	`$ gzserver`

Then finally, start the client with the CameraControl plugin:

	`$ gzclient -g libcameracontrol.so`

On the Oculus Rift
------------------
TBD