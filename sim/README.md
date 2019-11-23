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
