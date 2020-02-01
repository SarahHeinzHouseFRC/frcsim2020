/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_HUD_H
#define ROBOT_SIM_HUD_H


#include <vector>
#include <osg/Camera>
#include <osgText/Text>
#include "Label.h"
#include "VehicleModel.h"
#include "ConfigReader.h"
#include "Types.h"


class Hud
{
public:
    /**
     * Constructor
     */
    Hud(const ConfigReader& config);

    /**
     * Returns the HUD's camera
     */
    osg::Camera* getCamera() { return _camera; }

    /**
     * Callback called by the visualizer
     * @param width New window width (pixels)
     * @param height New window height (pixels)
     */
    void onWindowResize(int width, int height);

    /**
     * Displays "connected" or "disconnected"
     */
    void displayConnectionStatus(bool isConnected);

    /**
     * Displays robot state
     */
    void displayVehicleState(const VehicleModel& vehicleModel);

    /**
     * Displays the countdown timer
     * @param timerValue Current value (sec)
     */
    void displayTimer(double timerValue);

    /**
     * Displays number of collisions
     */
    void displayNumCollisions(int numCollisions);

private:
    osg::Camera* _camera;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::Geode> _geode;
    osg::ref_ptr<osg::Geode> _labelsGeode;
    osg::ref_ptr<osg::Geometry> _geom;
    osg::ref_ptr<osg::Vec3Array> _vertices;
    int _width;
    std::vector<Label*> _labels;
    osg::ref_ptr<Label> _connected;
    osg::ref_ptr<Label> _vehiclePoseState;
    osg::ref_ptr<Label> _vehicleElevatorState;
    osg::ref_ptr<Label> _vehicleDrivetrainState;
    osg::ref_ptr<Label> _timer;
    osg::ref_ptr<Label> _numCollisions;
};


#endif //ROBOT_SIM_HUD_H
