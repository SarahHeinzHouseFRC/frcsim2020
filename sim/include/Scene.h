/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"


class Scene
{
public:
    /**
     * Constructs the scene
     */
    Scene(const ConfigReader& config);

    /**
     * Renders the scene
     */
    void render();

    /**
     * Sets the elevator position where 0 is the bottom
     * @param pos Height (meters)
     */
    void setElevatorPosition(float pos);

private:
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::PositionAttitudeTransform> _carriagePat;
    float _beltRadius;
    float _beltWidth;
    float _beltLength;
    float _motorShaftRadius;
    float _motorShaftLength;
    float _motorRadius;
    float _motorLength;
    float _encoderRadius;
    float _encoderLength;
    float _carriageLengthX;
    float _carriageLengthY;
    float _carriageLengthZ;
};


#endif //ROBOT_SIM_SCENE_H
