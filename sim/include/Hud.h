/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once


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
    Hud(const ConfigReader& config, int playerId=-1);

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
     * Displays "connected" or "disconnected" for each player on the sim HUD
     */
    void displayConnectionStatus(const std::vector<bool>& connected);

    /**
     * Displays "connected" or "disconnected" for a single player on the sim view HUD
     * @param connected
     */
    void displayConnectionStatus(bool connected, int playerId);

    /**
     * Displays the countdown timer
     * @param running Whether timer is currently counting down or not
     * @param timerValue Current value (sec)
     */
    void displayTimerStatus(bool running, double timerValue);

    /**
     * Displays score
     */
    void displayFieldScore(int blueScore, int redScore);

    /**
     * Displays vehicle state
     */
    void displayVehicleState(const SimState& state, int playerId);

private:
    osg::Camera* _camera;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::Geode> _geode;
    osg::ref_ptr<osg::Geode> _labelsGeode;
    osg::ref_ptr<osg::Geometry> _geom;
    osg::ref_ptr<osg::Vec3Array> _vertices;
    int _width;
    std::vector<HudLabel*> _connectedLabels;
    std::vector<HudBox*> _connectedBoxes;
    osg::ref_ptr<HudLabel> _timer;
    osg::ref_ptr<HudLabel> _blueScore;
    osg::ref_ptr<HudLabel> _redScore;
    osg::ref_ptr<HudLabel> _vehiclePoseState;
    osg::ref_ptr<HudLabel> _vehicleDrivetrainState;
    float _padding = 15;
};
