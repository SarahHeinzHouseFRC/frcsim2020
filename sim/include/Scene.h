/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>


class Scene
{
public:
    /**
     * Constructs the scene
     */
    Scene();

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
    float _beltRadius = 0.02;
    float _beltWidth = 0.02;
    float _beltLength = 0.3;
    float _motorShaftRadius = 0.005;
    float _motorShaftLength = 0.01;
    float _motorRadius = 0.015;
    float _motorLength = 0.03;
    float _encoderRadius = 0.01;
    float _encoderLength = 0.003;
    float _carriageLengthX = 0.001;
    float _carriageLengthY = 0.03;
    float _carriageLengthZ = _beltWidth;
};


#endif //ROBOT_SIM_SCENE_H
