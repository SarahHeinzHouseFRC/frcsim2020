## Interface Control Document (ICD) ##
This document defines the interfaces between all four components of the simulator. They communicate over UDP using JSON
messages. JSON was used to make the interface easily extensible and allow others to develop more components that plug
into this simulator, for example, from ROS.

```
Port number:    2000             4000             6000            8000             10000            12000
+----------------+    Commands    +----------------+   Commands     +----------------+     State      +----------------+
|                | -------------> |                | -------------> |                | -------------> |                |
|   Joystick     |                |      Core      |                |   Vehicle Sim  |                |    Sim View    |
|                | <------------- |                | <------------- |                | <------------- |                |
+----------------+    Heartbeat   +----------------+     State      +----------------+   Heartbeat    +----------------+
```

### Joystick -> Core ###
The joystick sends the user's commands to the robot's core software. In the default configuration, these commands are
sent to localhost:4000. A sample of this message string follows:
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
The core sends back an empty JSON message to the joystick. This serves as a "heartbeat" to let the joystick know whether
the core is still alive. This allows the joystick to display a "connected" or "disconnected" message in its
GUI. The message has the following format:
```json5
{}
```

### Core -> Sim ###
The core logic performs whatever computations it wants (PID, traction control, etc.) using the commands from the
joystick and the vehicle's state from the sim as raw inputs and then constructs new commands to send to the vehicle. The
message sent to the vehicle has the following form:
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
The vehicle continuously sends vehicle and sensor state information back to the core software. This message has the
following form:
```json5
{
    "leftDriveEncoder": 0,    // Left drive encoder ticks (0 - 1024)
    "rightDriveEncoder": 0,   // Right drive encoder ticks (0 - 1024)
    "lidarSweep": [           // LIDAR sweep
        {
            "azimuth": 0.0,   // Point in sensor frame azimuth (rads 0 - 2pi)
            "elevation": 0.0, // Point in sensor frame elevation (rads 0)
            "range": 0.0      // Point in sensor frame range (meters)
        },
        // Etc.
    ]
}
```

### Sim -> Sim View ###
```json5
{
    "blueScore": 0,                        // Blue alliance score
    "redScore": 0,                         // Red alliance score
    "isTimerRunning": false,               // True for running, false for not paused
    "timer": 0.0,                          // Current value left on timer value (sec)
    "field": {
        "inCollision": false               // True for collision with field, false otherwise
    },
    "vehicles": [
        {
            "team": "3260",                // Team name
            "alliance": "Blue",            // Alliance ("Blue" or "Red")
            "x": 0.0,                      // Vehicle position (meters)
            "y": 0.0,                      // Vehicle position (meters)
            "theta": 0.0,                  // Vehicle heading (radians, not wrapped)
            "intakeCenterMotorSpeed": 0.0, // Center intake motor speed (rads/sec)
            "intakeLeftMotorSpeed": 0.0,   // Left intake motor speed (rads/sec)
            "intakeRightMotorSpeed": 0.0,  // Right intake motor speed (rads/sec)
            "tubeMotorSpeed": 0.0,         // Tube motor speed (rads/sec)
            "leftDriveMotorSpeed": 0.0,    // Left drive motor speed (rads/sec)
            "rightDriveMotorSpeed": 0.0,   // Right drive motor speed (rads/sec)
            "lidarSweep": [
                {
                    "azimuth": 0.0,        // Azimuth (0 - 2pi rads)
                    "elevation": 0.0,      // Elevation (0)
                    "range": 0.0           // Range (meters)
                },
                // Etc.
            ]
        }
    ],
    "gamePieces": [
        {
            "x": 0.0,                      // Game piece position (meters)
            "y": 0.0,                      // Game piece position (meters)
            "z": 0.0,                      // Game piece position (meters)
            "ingestionState": 0            // 0,1,2,3
        },
        // Etc.
    ]
}
```
