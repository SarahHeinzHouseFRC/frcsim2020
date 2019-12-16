# Joystick #

This project is designed to emulate a joystick. Upon launching the application, the user can use the joystick to
translate keystrokes and mouse inputs into JSON, which will be sent over UDP to the controller. The controller will send
back a simple heartbeat signal to let the joystick know it's connected.

This project uses Python 2.7 and is built on top of PyQt4.
