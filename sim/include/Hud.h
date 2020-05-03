/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_HUD_H
#define ROBOT_SIM_HUD_H


#include <vector>
#include <osg/Camera>
#include <osgText/Text>
#include "HudElement.h"
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
     * Displays the countdown timer
     * @param running Whether timer is currently counting down or not
     * @param timerValue Current value (sec)
     */
    void displayTimerStatus(bool running, double timerValue);

    /**
     * Displays score
     */
    void displayFieldScore(std::tuple<int, int> score);

    /**
     * Displays robot state
     */
    void displayVehicleState(const VehicleModel& vehicleModel);

private:
    osg::Camera* _camera;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::Geode> _geode;
    osg::ref_ptr<osg::Geode> _labelsGeode;
    osg::ref_ptr<osg::Geometry> _geom;
    osg::ref_ptr<osg::Vec3Array> _vertices;
    int _width;
    osg::ref_ptr<HudLabel> _connected;
    osg::ref_ptr<HudLabel> _timer;
    osg::ref_ptr<HudLabel> _blueScore;
    osg::ref_ptr<HudLabel> _redScore;
    osg::ref_ptr<HudLabel> _vehiclePoseState;
    osg::ref_ptr<HudLabel> _vehicleDrivetrainState;
    float _padding = 15;
};


#endif //ROBOT_SIM_HUD_H
