# Controller #

This project is designed to emulate an Xbox controller. Upon launching the application, the user can click the joystick
to translate mouse inputs into JSON, which will be sent over UDP to the controls. The controls will send back a simple
heartbeat signal to let the joystick know it's connected.

This project uses Python 2.7 and is built on top of PyQt4.
