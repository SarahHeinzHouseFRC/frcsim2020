# SHARP Team 3260 FRC Simulator #
This project consists of core robot code (Java), a mock joystick controller (Python), and a vehicle simulator (C++)
which communicate over fast UDP using JSON strings. This code was tested and runs in Ubuntu 18.04.

Among other benefits, this software allows the software team to rapidly develop and test their software independently of
the hardware team, status of the robot, and availability of physical joysticks; game strategy development; driver
tryouts; and more. The hope is to keep adding more functionality to this software, including log playback to allow log
playback for debugging, races against your own "phantom" from a previous run, and multiplayer support.

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

## Installation and Running ##
### Joystick ###
The joystick uses Python 2.7 and is built on top of PyQt4. The physical controller support is provided by `xboxdrv`. To
install, run:
```sh
sudo apt install python-pip
pip install PyYAML
sudo apt install python-qt4 # Needed for virtual controller
sudo apt install xboxdrv # Needed for virtual controller
```
To launch the virtual controller, run:
```sh
cd joystick/
python main.py virtual
```
To launch with a physical controller, plug in the controller to a USB port and run:
```sh
cd joystick/
sudo python main.py physical # Sudo is an unfortunate side effect of xboxdrv, hopefully remove this soon
```
Unfortunately, the Xbox controller library currently used in this application requires sudo permissions to launch. This
will hopefully be fixed soon by using the xbox360controller Python package instead, which supports more controllers,
other features such as rumble, and does not require superuser permissions.

To see all options in the joystick application, use:
```sh
python main.py virtual --help
```

### Core ###
JDK is required to compile the core. To install it, run
```sh
sudo apt install default-jdk
```
To compile and run the core, run:
```sh
cd core/
javac *.java
java Main
```

### Sim ###
To install the dependencies for the sim, run:
```sh
sudo apt install cmake libopenscenegraph-3.4-dev libyaml-cpp-dev
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
    fieldModelFile: "/your/path/to/field2020.wrl"         # Path to 3D WRL model of the field
    vehicleModelFile: "/your/path/to/vehicle2020.wrl"     # Path to 3D WRL model of the vehicle
    gamePieceModelFile: "/your/path/to/gamepiece2020.wrl" # Path to 3D WRL model of the game piece
    fontFile: "/your/path/to/helvetica.ttf"               # Path to font file for HUD text
```

To compile and run the sim, run:
```sh
cd sim/
mkdir build
cd build/
cmake ..
make -j8
./robot_sim # This can take >30s because the 3D models are so large
```
Because the CAD models for the robot and field are so large, you can optionally launch the simulator with a lightweight
visualization using:
```sh
./robot_sim --debug-view
```
To see all options available in the sim, use:
```sh
./robot_sim --help
```


## Configuration File ##
All three applications refer to the same config file at launch, which is located at `config/robotConfig.yml`. Options
for changing the IPs and ports are available, as well as some configuration options to change the vehicle and field
shapes.


## Interface Control Document (ICD) ##
This section defines the interfaces between all three components of the simulator. They communicate over fast UDP using
a JSON-ish specification. Eventually this will move to real JSON, but for now I took a shortcut and implemented
something simple to get started. Feel free to extend the interface and develop more components that plug into this
simulator.

### Joystick -> Core ###
The joystick sends the user's commands to the robot's core logic over UDP as JSON-ish strings. In the default
configuration, these commands are sent to localhost:4000. A sample of this JSON string follows:
```json
{ 'leftJoystick': [ 0000, 0000 ], 'rightJoystick': [ 0000, 0000 ], 'dpad': [ 0, 0, 0, 0 ], 'buttons': [ 0, 0, 0, 0 ], 'back': 0, 'select': 0, 'start': 0 }
```

| Character(s)  | Description                     | Range      |
| --------------| ------------------------------- | ---------- |
| 20-24         | Left joystick's x-displacement  | -512 - 512 |
| 26-30         | Left joystick's y-displacement  | -512 - 512 |
| 53-57         | Right joystick's x-displacement | -512 - 512 |
| 59-63         | Right joystick's y-displacement | -512 - 512 |
| 77-78         | Dpad up                         | 0 or 1     |
| 80-81         | Dpad down                       | 0 or 1     |
| 83-84         | Dpad left                       | 0 or 1     |
| 86-87         | Dpad right                      | 0 or 1     |
| 104-105       | A button                        | 0 or 1     |
| 107-108       | B button                        | 0 or 1     |
| 110-111       | X button                        | 0 or 1     |
| 113-114       | Y button                        | 0 or 1     |
| 126-127       | Back button                     | 0 or 1     |
| 139-140       | Select button                   | 0 or 1     |
| 151-152       | Start button                    | 0 or 1     |

### Core -> Joystick ###
The core logic also sends back an empty JSON string to the joystick of the following format:
```json
{}
```
The purpose of this empty message is to serve as a "heartbeat" to let the joystick know whether the controls logic is
still alive. This allows the joystick to display a "connected" or "disconnected" message in its GUI.

### Core -> Sim ###
The core logic performs whatever logic (PID, traction control, etc.) given the commands from the joystick and the
vehicle's state from the sim and then construct new commands to send to the vehicle. The message sent to the vehicle has
the following form:
```json
{ 'leftDriveMotorSpeed': 0000, 'rightDriveMotorSpeed': 0000, 'intakeCenterMotorSpeed': 0000, 'intakeLeftMotorSpeed': 0000, 'intakeRightMotorSpeed': 0000, 'tubeMotorSpeed': 0000, 'timerStartStop': 0, 'reset': 0 }
```

| Character(s)  | Description                     | Range      |
| --------------| ------------------------------- | ---------- |
| 25-29         | Left drive motor speed          | -512 - 512 |
| 55-59         | Right drive motor speed         | -512 - 512 |
| 87-91         | Intake center motor speed       | -512 - 512 |
| 117-121       | Intake left motor speed         | -512 - 512 |
| 148-152       | Intake right motor speed        | -512 - 512 |
| 172-176       | Tube motor speed                | -512 - 512 |
| 196-197       | Timer start/stop                | 0 or 1     |
| 208-209       | Reset                           | 0 or 1     |

### Sim -> Core ###
The vehicle continuously sends state information back to the controls logic. This message has the following form:
```json
{ 'leftDriveEncoder': 0000, 'rightDriveEncoder': 0000, 'elevatorEncoder': 0000 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 22-26         | Left drive encoder ticks        | 0 - 1024 |
| 49-53         | Right drive encoder ticks       | 0 - 1024 |
| 74-78         | Elevator encoder ticks          | 0 - 1024 |
