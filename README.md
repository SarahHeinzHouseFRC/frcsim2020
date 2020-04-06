# FRC Simulator 2020 #
## Team SHARP 3260 ##

![A simulated robot drives around the field using a virtual Xbox controller](https://prateeksahay.com/img/frcsim.gif)

## Overview ##

The purpose of this project is to allow an FRC software team to rapidly develop and test their software independently of
the hardware team, availability of the robot, and availability of physical joysticks. Other benefits include game
strategy development; driver tryouts; and more. The project consists of four discrete components, and the APIs for each
are available in an [ICD](docs/icd.md) to promote extensibility of this software, including support for log playback for
debugging, racing against your own "phantom" from a previous run, and multiplayer support.

The four components are the core robot code (Java), a mock Xbox controller (Python), a vehicle simulator (C++), and
additional views of the field (C++), which all communicate over UDP using JSON strings. This code was tested and run in
Ubuntu 18.04.

For details on each of the components, please see the specific documentation for each:
  - [Joystick](joystick/README.md)
  - [Core](core/README.md)
  - [Sim and Sim View](sim/README.md)

## Feature List ##
  - [x] Fully-featured PyQt-based virtual Xbox controller to use for local development.
  - [x] Support for physical Xbox 360 controller over USB.
  - [x] Simulated robot with working drivetrain, intake, and outtake using real-world motor velocities and gearing.
  - [x] Easy YAML configuration.
  - [x] Human-readable UDP comms using human-readable strings for easy debugging and extension.
  - [x] OpenSceneGraph-based 3D visualization of sim with included CAD models of the robot and field.
  - [x] Realistic 2.5D physics built on top of the popular [Box2D](https://box2d.org/) physics library.
  - [x] Match timer and reset functionality of field and robot for practicing matches/driver tryouts.
  - [x] Support for up to 6 simultaneous robots.
  - [x] Realistic LIDAR model.
  - [ ] Support for Xbox One, PS4, and other controllers.
  - [ ] Log replay and mode to play against your own phantom.
  - [ ] 3D physics engine.

## Documentation ##
  - [Building and Running](docs/build.md)
  - [Configuration](docs/config.md)
  - [Interface Control Document](docs/icd.md)
