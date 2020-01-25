# SHARP Team 3260 FRC Software #
Consists of robot code, a mock joystick controller, and a simulator.

## Interface Control Document (ICD) ##

```
      +----------------+      Commands      +----------------+     Commands       +----------------+
      |                | -----------------> |                | -----------------> |                |
      |   Joystick     |                    |    Controls    |                    |     Vehicle    |
      |                | <----------------- |                | <----------------- |                |
      +----------------+     Heartbeat      +----------------+       State        +----------------+
```

### Joystick -> Controls ###
The joystick sends the user's commands to the robot controls logic as JSON over UDP. In the default configuration, these
commmands are sent to localhost:4000. A sample of this JSON string follows:
```
{ 00000 00000 00000 00000 0000 0000 }
```

| Character(s)  | Descrption                      | Range    |
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

### Controls -> Joystick ###
The controls logic also sends back an empty JSON string to the joystick of the following format:
```
{}
```
The purpose of this empty message is to serve as a "heartbeat" to let the joystick know whether the controls logic is
still alive. This allows the joystick to display a "connected" or "disconnected" message in its GUI.

### Controls -> Vehicle ###
The controls logic can perform whatever logic (PID, traction control, etc.) on the received joystick commands and then
construct new commands to send to the vehicle. The message sent to the vehicle has the following form:
```
{ 00000 00000 00000 }
```

| Character(s)  | Descrption                      | Range    |
| --------------| ------------------------------- | -------- |
| 3-7           | Left drive motor speed          | -511-512 |
| 9-13          | Right drive motor speed         | -511-512 |
| 15-19         | Elevator motor speed            | -511-512 |

### Vehicle -> Controls ###
The vehicle continuously sends state information back to the controls logic. This message has the following form:
```
{ 00000 00000 00000 }
```

| Character(s)  | Descrption                      | Range    |
| --------------| ------------------------------- | -------- |
| 3-7           | Left drive encoder ticks        | 0-1024   |
| 9-13          | Right drive encoder ticks       | 0-1024   |
| 15-19         | Elevator encoder ticks          | 0-1024   |
