/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "RobotModel.h"


/**
 * Responsible for rendering the robot according to the robot model.
 */
class Scene
{
public:
    /**
     * Constructs the scene
     */
    Scene(const ConfigReader& config);

    /**
     * Updates the scene given the robot's current state
     */
    void update(const RobotModel& robot);

    /**
     * Returns the root node of the scene
     */
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

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
