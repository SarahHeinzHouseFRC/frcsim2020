# Vehicle Simulation #

This component of the project attempts to simulate the vehicle, field, and game pieces of the 2020 game. The simulation
is the largest and most complicated application in this project.

The front end is currently rendered using OpenSceneGraph (OSG), but there is a future possibility of switching over to
Unreal Engine. The field and robot models are exported from SolidWorks as `.wrl` files, which happen to be supported by
both SolidWorks and OSG. However, these CAD models are fairly large and can take some time to launch. For faster
debugging, there is an option to launch with a simplified view by using the `--debug-view` option.

The backend consists of models of the field, vehicle, and game pieces. A physics engine called Box2D (famous for its use
in Angry Birds) performs the collision-checking and constraint-solving, which is included in this project and built as a
statically-linked library.

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
./robot_sim <optional/path/to/config/file>
```

## Usage ##

OSG's origin (0, 0, 0) is at the center of the field. The robot follows OSG's convention and faces in the positive-x
direction with the z-axis pointing up when its theta is 0. Therefore, a positive rotation is to the left of the x-axis
as expected according to the right-hand rule.
