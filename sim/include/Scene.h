/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/ShapeDrawable>
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
    /**
     * Builds the visualization of the robot
     */
    osg::ref_ptr<osg::PositionAttitudeTransform> makeRobot(const ConfigReader& config);

    /**
     * Builds the visualization of the robot's elevator carriage
     */
    osg::ref_ptr<osg::PositionAttitudeTransform> makeRobotCarriage();

    /**
     * Builds the visualization of the field
     */
    osg::ref_ptr<osg::Geode> makeField();

    /**
     * Utility method for quickly drawing lines. Pairs of vertices are interpreted as single lines
     */
    static osg::ref_ptr<osg::Geometry> makeLines(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a cylinder
     */
    static osg::ref_ptr<osg::ShapeDrawable> makeCylinder(const osg::Vec3& pos, float radius, float height, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a box
     */
    static osg::ref_ptr<osg::ShapeDrawable> makeBox(const osg::Vec3& pos, float lengthX, float lengthY, float lengthZ, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing quads
     */
    static osg::ref_ptr<osg::Geometry> makeQuads(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::PositionAttitudeTransform> _robotPat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _carriagePat;
    float _wheelRadius;
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
