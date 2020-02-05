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
```
{ 00000 00000 00000 00000 0000 0000 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 2-7           | Left joystick's x-displacement  | -511-512 |
| 8-13          | Left joystick's y-displacement  | -511-512 |
| 14-19         | Right joystick's x-displacement | -511-512 |
| 20-25         | Right joystick's x-displacement | -511-512 |
| 26            | A button                        | 0 or 1   |
| 27            | B button                        | 0 or 1   |
| 28            | X button                        | 0 or 1   |
| 29            | Y button                        | 0 or 1   |
| 31            | Dpad up                         | 0 or 1   |
| 32            | Dpad down                       | 0 or 1   |
| 33            | Dpad left                       | 0 or 1   |
| 34            | Dpad right                      | 0 or 1   |

### Core -> Joystick ###
The core logic also sends back an empty JSON string to the joystick of the following format:
```
{}
```
The purpose of this empty message is to serve as a "heartbeat" to let the joystick know whether the controls logic is
still alive. This allows the joystick to display a "connected" or "disconnected" message in its GUI.

### Core -> Vehicle ###
The core logic performs whatever logic (PID, traction control, etc.) given the commands from the joystick and its model
of the vehicle's state and then construct new commands to send to the vehicle. The message sent to the vehicle has the
following form:
```
{ 00000 00000 00000 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 3-7           | Left drive motor speed          | -511-512 |
| 9-13          | Right drive motor speed         | -511-512 |
| 15-19         | Elevator motor speed            | -511-512 |

### Vehicle -> Core ###
The vehicle continuously sends state information back to the controls logic. This message has the following form:
```
{ 00000 00000 00000 }
```

| Character(s)  | Description                     | Range    |
| --------------| ------------------------------- | -------- |
| 3-7           | Left drive encoder ticks        | 0-1024   |
| 9-13          | Right drive encoder ticks       | 0-1024   |
| 15-19         | Elevator encoder ticks          | 0-1024   |
