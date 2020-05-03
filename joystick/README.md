# Joystick #

The joystick code is designed to give inputs to the core software. It can run in either "virtual" or "physical" mode,
meaning the user can choose to either launch with a physical Xbox 360 controller plugged into a USB port on this
computer or as a PyQt application that emulates an Xbox 360 controller. The library used for supporting the physical
controller was written by FRC Team 4564 in 2013. Note that launching with a physical controller currently requires sudo
privileges, which I hope so solve soon.

Regardless of physical or virtual, the joystick has two threads: one for grabbing the state of the controller and one
for the UDP comms layer.
