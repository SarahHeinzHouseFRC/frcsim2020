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

## Configuration File ##
This project can be configured using the YAML config file located at `config/robotConfig.yml`. Options for changing the
IPs and ports are available, as well as some configuration options for the vehicle dynamics.


## Interface Control Document (ICD) ##
Feel free to develop more components that plug into this simulator. The interface is open and explained in detail below.

### Joystick -> Core ###
The joystick sends the user's commands to the robot's core logic as JSON over UDP. In the default configuration, these
commands are sent to localhost:4000. A sample of this JSON string follows:
```json
{ 'leftJoystick': [ 0000, 0000 ], 'rightJoystick': [ 0000, 0000 ], 'dpad': [ 0, 0, 0, 0 ], 'buttons': [ 0, 0, 0, 0 ], 'back': 0, 'select': 0, 'start': 0 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 20-24         | Left joystick's x-displacement  | -512-512 |
| 26-30         | Left joystick's y-displacement  | -512-512 |
| 53-57         | Right joystick's x-displacement | -512-512 |
| 59-63         | Right joystick's y-displacement | -512-512 |
| 77-78         | Dpad up                         | 0 or 1   |
| 80-81         | Dpad down                       | 0 or 1   |
| 83-84         | Dpad left                       | 0 or 1   |
| 86-87         | Dpad right                      | 0 or 1   |
| 104-105       | A button                        | 0 or 1   |
| 107-108       | B button                        | 0 or 1   |
| 110-111       | X button                        | 0 or 1   |
| 113-114       | Y button                        | 0 or 1   |
| 126-127       | Back button                     | 0 or 1   |
| 139-140       | Select button                   | 0 or 1   |
| 151-152       | Start button                    | 0 or 1   |

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
{ 'leftDriveMotorSpeed': 0000, 'rightDriveMotorSpeed': 0000, 'elevatorMotorSpeed': 0000, 'back': 0, 'guide': 0, 'start': 0 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 3-7           | Left drive motor speed          | -511-512 |
| 9-13          | Right drive motor speed         | -511-512 |
| 15-19         | Elevator motor speed            | -511-512 |

### Sim -> Core ###
The vehicle continuously sends state information back to the controls logic. This message has the following form:
```json
{ 'leftDriveEncoder': 0000, 'rightDriveEncoder': 0000, 'elevatorEncoder': 0000 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 22-26         | Left drive encoder ticks        | 0-1024   |
| 49-53         | Right drive encoder ticks       | 0-1024   |
| 74-78         | Elevator encoder ticks          | 0-1024   |
