/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_HUD_H
#define ROBOT_SIM_HUD_H


#include <vector>
#include <osg/Camera>
#include <osgText/Text>
#include "Label.h"
#include "RobotModel.h"
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
    void displayConnected(bool isConnected);

    /**
     * Displays robot state
     */
    void displayRobotState(const RobotModel& robotModel);

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
    osg::ref_ptr<Label> _robotState;
};


#endif //ROBOT_SIM_HUD_H
