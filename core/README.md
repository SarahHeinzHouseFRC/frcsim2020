# Core #

The core software is meant to allow plug-and-play functionality with the code developed for the physical robot. The
logic in this component combines the joystick inputs with the vehicle sim's sensor readings to produce intelligent
outputs to the vehicle sim. This logic can be, for example, intelligent autonomous behavior, traction control, and
control loops.

The core runs two threads: one for constructing commands for the vehicle given the current vehicle state and joystick
inputs, and one for the UDP comms layer.
