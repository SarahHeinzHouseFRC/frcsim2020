# Robot Simulation #

This project is a simple simulator for the robot. It allows the software team to develop and test their software
independently of the hardware. It may also provide functionality in the future for replaying logs recorded by the actual
robot.

## Building and Running ##

First, install the required C++ dependencies:
```sh
sudo apt install libopenscenegraph-3.4-dev libyaml-dev
```

Then, build the project with:
```sh
mkdir build
cd build
cmake ..
make -j8
```

Finally, launch the project with the default config file:
```sh
./robot_sim ../config/simConfig.yml
```

## Usage ##

OSG's origin (0, 0, 0) is at the center of the field. The robot follow's OSG's convention and faces in the positive-x
direction with the z-axis pointing up. Therefore, a positive rotation is to the left of the x-axis.

The robot's CoG can be customized. By default, it is placed at the robot's origin exactly halfway between its two middle
wheels.
