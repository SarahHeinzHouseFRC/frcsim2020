# SHARP Team 3260 FRC Simulator #
This project consists of the core robot code (Java), a mock Xbox controller (Python), and a vehicle simulator (C++)
which communicate over UDP using JSON strings. This code was tested and run in Ubuntu 18.04.

Among many other benefits, this software allows an FRC software team to rapidly develop and test their software
independently of the hardware team, availability of hte robot, and availability of physical joysticks; game strategy
development; driver tryouts; and more. The APIs to each component are intentionally made available to promote
extensibility of this software, including support for log playback for debugging, racing against your own "phantom" from
a previous run, and multiplayer support.

For details on each of the three components included, please see the specific documentation for each:
  - [Joystick](joystick/README.md)
  - [Core](core/README.md)
  - [Sim](sim/README.md)

```
      +----------------+      Commands      +----------------+     Commands       +----------------+
      |                | -----------------> |                | -----------------> |                |
      |   Joystick     |                    |      Core      |                    |     Vehicle    |
      |                | <----------------- |                | <----------------- |                |
      +----------------+     Heartbeat      +----------------+       State        +----------------+
```


## Feature List ##
  - [x] Fully-featured PyQt-based virtual Xbox controller that publishes commands over UDP as JSON.
  - [x] Support for physical Xbox 360 controller over USB.
  - [x] Simulated robot with working drivetrain, intake, and outtake using real-world motor velocities and gearing.
  - [x] Easy YAML configuration.
  - [x] Human-readable UDP comms using human-readable strings for easy debugging and extension.
  - [x] OpenSceneGraph-based 3D visualization with included CAD models of the robot and field.
  - [x] Realistic 2.5D physics built on top of the popular [Box2D](https://box2d.org/) physics library.
  - [x] Match timer and reset functionality of field and robot for practicing matches/driver tryouts.
  - [ ] Support for Xbox One, PS4, and other controllers.
  - [ ] Support for up to 6 simultaneous robots.
  - [ ] Log replay and mode to play against your own phantom.
  - [ ] Move to a 3D physics engine (NVIDIA PhysX or ODE).


## Installation and Running ##
### Joystick ###
The joystick uses Python 2.7 and is built on top of PyQt4. The physical controller support is provided by `xboxdrv`. To
install, run:
```sh
sudo apt install python-pip
pip install PyYAML
sudo apt install python-qt4 # Can skip this if only running virtual controller
sudo apt install xboxdrv    # Can skip this if only running physical controller
```
To launch the virtual controller, run:
```sh
cd joystick/
python main.py virtual --player $N # Replace $N with a number 1-6
```
To launch with a physical controller, plug in the controller to a USB port and run:
```sh
cd joystick/
sudo python main.py physical --player $N # Sudo is an unfortunate side effect of xboxdrv, hopefully remove this soon
```
Unfortunately, the Xbox controller library currently used in this application requires sudo permissions to launch. This
will hopefully be fixed soon by using the xbox360controller Python package instead, which supports more controllers,
other features such as rumble, and does not require superuser permissions.

To see all options in the joystick application, use:
```sh
python main.py virtual --help
```

### Core ###
JDK and Gradle are required to compile the core. To install them, run
```sh
sudo apt install openjdk-8-jdk-headless
```
To compile and run the core, run:
```sh
cd core/
./gradlew run --args='--player $N' # Replace $N with a number 1-6
```

### Sim ###
To install the dependencies for the sim, run:
```sh
sudo apt install cmake libopenscenegraph-3.4-dev libyaml-cpp-dev rapidjson-dev
```
To download the (optional) visual assets, download each from here:
  - [Field 3D model](https://www.dropbox.com/s/1p1i1cbpkj8jp9x/field2020.wrl?dl=0)
  - [Robot 3D model](https://www.dropbox.com/s/ksr9qn4lipebdw8/vehicle2020.wrl?dl=0)
  - [Game piece 3D model](https://www.dropbox.com/s/crgws9oz5v3tcpp/gamepiece2020.wrl?dl=0)
  - [Helvetica font](https://www.dropbox.com/s/2a4qm5csm96wcku/helvetica.ttf?dl=0)

You will also need to adjust the following parameters in your config file to let the sim know where you downloaded each
one of these files:
```yaml
sim:
  assets:
    fieldModelFile: "/your/path/to/field2020.wrl"         # Absolute path to 3D WRL model of the field
    vehicleModelFile: "/your/path/to/vehicle2020.wrl"     # Absolute path to 3D WRL model of the vehicle
    gamePieceModelFile: "/your/path/to/gamepiece2020.wrl" # Absolute path to 3D WRL model of the game piece
    fontFile: "/your/path/to/helvetica.ttf"               # Absolute path to font file for HUD text
```

To compile and run the sim, run:
```sh
cd sim/
mkdir build
cd build/
cmake ..
make -j8
./robot_sim # Note that this can take >30s to launch because the 3D models are so large, optionally use --debug-view
```
Because the CAD models for the robot and field are large and can slow down the launch time and runtime performance of
the visualization, you can optionally launch the simulator with a lightweight visualization using:
```sh
./robot_sim --debug-view
```
To launch with more players, use:
```sh
./robot_sim --players $N # Replace $N with a number 1-6
```
To see all options available in the sim, use:
```sh
./robot_sim --help
```


## Configuration File ##
All three applications refer to the same YAML config file at launch, which is located at `config/robotConfig.yml`.
Options for changing the IPs and ports are available, as well as some configuration options to change the vehicle and
field shapes.


## Interface Control Document (ICD) ##
This section defines the interfaces between all three components of the simulator. They communicate over UDP using JSON
messages. JSON was used to make the interface easily extensible and allow others to develop more components that plug
into this simulator, for example, from ROS.


### Joystick -> Core ###
The joystick sends the user's commands to the robot's core logic over UDP as JSON-ish strings. In the default
configuration, these commands are sent to localhost:4000. A sample of this message string follows:
```json5
{
    "leftJoystick": [
        0,             // Left joystick's x-displacement (-512 - 512)
        0              // Left joystick's y-displacement (-512 - 512)
    ],
    "rightJoystick": [
        0,             // Right joystick's x-displacement (-512 - 512)
        0              // Right joystick's y-displacement (-512 - 512)
    ],
    "dpad": [
        0,             // Dpad up (0 or 1)
        0,             // Dpad down (0 or 1)
        0,             // Dpad left (0 or 1)
        0              // Dpad right (0 or 1)
    ],
    "leftTrigger": 0,  // Left trigger (0 - 512)
    "rightTrigger": 0, // Right trigger (0 - 512)
    "leftBumper": 0,   // Left bumper (0 or 1)
    "rightBumper": 0,  // Right bumper (0 or 1)
    "a": 0,            // A button (0 or 1)
    "b": 0,            // B button (0 or 1)
    "x": 0,            // X button (0 or 1)
    "y": 0,            // Y button (0 or 1)
    "back": 0,         // Back button (0 or 1)
    "start": 0,        // Guide button (0 or 1)
    "guide": 0         // Start button (0 or 1)
}
```

### Core -> Joystick ###
The core logic also sends back an empty JSON string to the joystick of the following format:
```json5
{}
```
The purpose of this empty message is to serve as a "heartbeat" to let the joystick know whether the controls logic is
still alive. This allows the joystick to display a "connected" or "disconnected" message in its GUI.

### Core -> Sim ###
The core logic performs whatever logic (PID, traction control, etc.) given the commands from the joystick and the
vehicle's state from the sim and then construct new commands to send to the vehicle. The message sent to the vehicle has
the following form:
```json5
{
    "leftDriveMotorSpeed": 0,    // Left drive motor speed (-512 - 512)
    "rightDriveMotorSpeed": 0,   // Right drive motor speed (-512 - 512)
    "intakeCenterMotorSpeed": 0, // Intake center motor speed (-512 - 512)
    "intakeLeftMotorSpeed": 0,   // Intake left motor speed (-512 - 512)
    "intakeRightMotorSpeed": 0,  // Intake right motor speed (-512 - 512)
    "tubeMotorSpeed": 0,         // Tube motor speed (-512 - 512)
    "timerStartStop": 0,         // Timer start/stop (0 or 1)
    "reset": 0,                  // Reset (0 or 1)
    "outtake": 0                 // Outtake (0 or 1)
}
```

### Sim -> Core ###
The vehicle continuously sends state information back to the controls logic. This message has the following form:
```json5
{
    "leftDriveEncoder": 0,  // Left drive encoder ticks (0 - 1024)
    "rightDriveEncoder": 0, // Right drive encoder ticks (0 - 1024)
    "elevatorEncoder": 0    // Elevator encoder ticks (0 - 1024)
}
```
