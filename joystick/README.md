# Joystick #

The joystick code is designed to give inputs to the core software. It can run in either "virtual" or "physical" mode,
meaning the user can either launch this application with a physical Xbox 360 controller plugged into a USB port on a
computer or as a PyQt application that emulates an Xbox 360 controller. The library used for supporting the physical
controller was written by FRC Team 4564 in 2013.

## Dependencies ##
This project uses Python 2.7 and is built on top of PyQt4. The physical controller support is provided by `xboxdrv`. To
install, run:
```
sudo apt install python-qt4-dev xboxdrv
```

## Running ##
To launch in virtual mode, run:
```
python main.py virtual
```
To launch in physical mode, plug your physical Xbox 360 controller into a USB port on your computer and run:
```
sudo python main.py physical
```
Unfortunately, the Xbox controller library used by this application currently requires sudo permissions to launch. This
will hopefully be fixed soon by using the xbox360controller Python package instead, which supports more controllers,
other features such as rumble, and does not require superuser permissions.
