# Vehicle Simulator and View #

The simulator component of the project attempts to simulate the vehicle, field, and game pieces of the 2020 FRC game.
The simulation is the largest and most complicated application in this project, so it also attempts to be the most
performant.

The sim view is an optional component of the simulator which is useful for multiplayer. If multiple users want to play
competitively using the simulator, they would each launch their own joystick, core, and view, then connect to a common
simulator. The sole purpose of the view is to allow each player to have a unique perspective of the same game, similar
to a windowed multiplayer view in a videogame.

The sim runs three threads: one for the field, game pieces, and vehicle simulation, one for the 3D visualization, and
one for the UDP comms layer.

The sim view runs two threads: one for the 3D visualization and one for the UDP comms layer.

The visualization is rendered using OpenSceneGraph (OSG), which is a lightweight C++ wrapper on top of OpenGL, but there
has been some exploration into switching to something more powerful like Unreal Engine, which has the additional
advantage of natively supporting SolidWorks assets. The field and robot models must currently be exported from
SolidWorks as `.wrl` files, which are supported by OSG. Note that these CAD models are fairly large and can take some
time to launch. For debugging, there is a command line option to launch with a simplified view by using the
`--debug-view`.

The backend consists of a model-view-controller framework comprising the field, vehicle, and game pieces. A physics
engine called [Box2D](https://box2d.org/) (famous for its use in Angry Birds) performs the collision detection and
dynamics, which is included in this project for convenience and built as a statically-linked library.

The center of the field is placed at OSG's (0, 0, 0) origin point. A point placed at (1, 0, 0) in OSG is 1 meter from
the origin. OSG's x-axis is parallel to the short edge of the field, y-axis is parallel to the long edge, and z-axis
points up. Note that this forms a right-hand coordinate system, meaning angles are measured counterclockwise from the
x-axis when looking down at the field. The robot can be configured to start in any 2D position and orientation using the
YAML config file. The robot CAD model is intentionally exported to obey OSG's coordinate system and faces in the
positive-x direction with the z-axis pointed up when its theta is 0.
