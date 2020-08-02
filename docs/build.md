## Building and Running ##

### Joystick ###
The joystick uses Python 2.7 and is built on top of PyQt4. The physical controller support is provided by `xboxdrv`. To
install, run:
```sh
sudo apt install python-pip
pip install PyYAML
sudo apt install python-qt4 # Can skip this if only running virtual controller
sudo apt install xboxdrv    # Can skip this if only running physical controller
```
To launch the virtual controller, run:
```sh
cd joystick/
python main.py virtual --player $N # Replace $N with a number 1-6
```
Unfortunately, the Xbox controller library currently used in this application requires sudo permissions to launch. This
will hopefully be fixed soon by using the xbox360controller Python package instead, which works with more controllers,
supports features such as rumble, and does not require superuser permissions. To launch with a physical controller, plug
in the controller to a USB port and run:
```sh
cd joystick/
sudo python main.py physical --player $N # Sudo is an unfortunate side effect of xboxdrv, hope to remove this soon
```

To see all options in the joystick application, use:
```sh
python main.py virtual --help
```

### Core ###
JDK is required to compile the core. To install a JDK, run
```sh
sudo apt install openjdk-8-jdk-headless
```
To automatically download Gradle, compile, and run the core, run:
```sh
cd core/
./gradlew run --args='--player $N' # Replace $N with a number 1-6
```

### Sim ###
To install the dependencies for the sim, run:
```sh
sudo apt install cmake libopenscenegraph-3.4-dev libyaml-cpp-dev rapidjson-dev libbox2d-dev
```
To download the (optional) visual assets, download each from here:
  - [Field 3D model](https://www.dropbox.com/s/1p1i1cbpkj8jp9x/field2020.wrl?dl=0)
  - [Blue robot 3D model](https://www.dropbox.com/s/1pobiyh0wz1eppm/vehicleBlue2020.wrl?dl=0)
  - [Red robot 3D model](https://www.dropbox.com/s/r0ymy4oo0xdvl2i/vehicleRed2020.wrl?dl=0)
  - [Game piece 3D model](https://www.dropbox.com/s/crgws9oz5v3tcpp/gamepiece2020.wrl?dl=0)
  - [Helvetica font](https://www.dropbox.com/s/2a4qm5csm96wcku/helvetica.ttf?dl=0)

You will also need to adjust the following parameters in your config file to let the sim know where you downloaded each
one of these files:
```yaml
sim:
  assets:
    fieldModelFile: "/your/absolute/path/to/field2020.wrl"             # Absolute path to 3D WRL model of the field
    vehicleBlueModelFile: "/your/absolute/path/to/vehicleBlue2020.wrl" # Absolute path to 3D WRL model of the vehicle
    vehicleRedModelFile: "/your/absolute/path/to/vehicleRed2020.wrl"   # Absolute path to 3D WRL model of the vehicle
    gamePieceModelFile: "/your/absolute/path/to/gamepiece2020.wrl"     # Absolute path to 3D WRL model of the game piece
    fontFile: "/your/absolute/path/to/helvetica.ttf"                   # Absolute path to font file for HUD text
```

To compile and run the sim, run:
```sh
cd sim/
mkdir build
cd build/
cmake ..
make -j8
./sim # Adding --debug-view will allow faster launching with simplified graphics
```
Because the CAD models for the robot and field are large and can slow down the launch time and runtime performance of
the visualization, you can optionally launch the simulator with a lightweight visualization using:
```sh
./sim --debug-view
```
If you'd prefer to give each player their own dedicated view, you can run the sim in an even more lightweight headless
mode, which doesn't launch any visualization at all, and launch individual sim views to visualize instead. To launch the
sim in headless mode, launch however many sim views you like and run:
```sh
./sim --headless
```
To see all options available in the sim, use:
```sh
./sim --help
```


## Sim View ##
The sim view is built using many of the same files as the sim. Therefore, the dependencies are the same as above,
including the 3D models and font file.

To compile and run the sim view, run:
```sh
cd sim/build
make -j8
./simView # Adding --debug-view will allow faster launching, but with simpler graphics
```
Because the 3D models for the robot and field are large and can slow down the launch time and runtime performance of the
visualization, you can optionally launch the sim view with a lightweight visualization using:
```sh
./simView --debug-view
```
Launching individual sim views for each player also allows you to snap to your vehicle's first-person view. To launch as
a different player, use:
```sh
./sim --player $N # Replace $N with a number 1-6
```
