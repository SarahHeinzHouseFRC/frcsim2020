/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <RobotModel.h>
#include <ConfigReader.h>
#include "Color.h"
#include "Scene.h"


Scene::Scene(const ConfigReader& config) :
        _wheelRadius(config.vehicle.constants.drivetrain.wheelRadius),
        _beltRadius(config.vehicle.constants.elevator.belt.radius),
        _beltWidth(config.vehicle.constants.elevator.belt.width),
        _beltLength(config.vehicle.constants.elevator.belt.length),
        _motorShaftRadius(config.vehicle.constants.elevator.motorShaft.radius),
        _motorShaftLength(config.vehicle.constants.elevator.motorShaft.length),
        _motorRadius(config.vehicle.constants.elevator.motor.radius),
        _motorLength(config.vehicle.constants.elevator.motor.length),
        _encoderRadius(config.vehicle.constants.elevator.encoder.radius),
        _encoderLength(config.vehicle.constants.elevator.encoder.length),
        _carriageLengthX(config.vehicle.constants.elevator.carriage.lengthX),
        _carriageLengthY(config.vehicle.constants.elevator.carriage.lengthY),
        _carriageLengthZ(config.vehicle.constants.elevator.carriage.lengthZ)
{
    _root = new osg::Group;

    _robotGeode = makeRobot(config);
    _robotPat = new osg::PositionAttitudeTransform;
    _robotPat->addChild(_robotGeode);
    _carriagePat = makeRobotCarriage();
    _robotPat->addChild(_carriagePat);
    _root->addChild(_robotPat);

    osg::ref_ptr<osg::Geode> field = makeField();
    _root->addChild(field);
}



void Scene::update(const RobotModel& robot)
{
    // Update elevator position
    double elevatorPos = robot._state.elevatorCarriagePos;
    _carriagePat->setPosition(osg::Vec3(0, 0, elevatorPos));

    // Update the position
    double x = robot._state.pose.x;
    double y = robot._state.pose.y;
    double theta = robot._state.pose.theta;
    _robotPat->setPosition(osg::Vec3(x, y, _wheelRadius));
    osg::Matrix mat;
    mat.makeRotate(theta, osg::Z_AXIS);
    _robotPat->setAttitude(mat.getRotate());
}



osg::ref_ptr<osg::Geode> Scene::makeRobot(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> robotGeode = new osg::Geode;

    //
    // Render the robot drivetrain
    //

    float widthChassis = config.vehicle.constants.drivetrain.width;
    float depthChassis = config.vehicle.constants.drivetrain.depth;
    float widthChannel = config.vehicle.constants.drivetrain.widthChannel;
    float heightChannel = config.vehicle.constants.drivetrain.heightChannel;
    float wheelWidth = config.vehicle.constants.drivetrain.wheelWidth;
    float wheelRadius = config.vehicle.constants.drivetrain.wheelRadius;
    float wheelBase = config.vehicle.constants.drivetrain.wheelBase;
    float wheelTrack = config.vehicle.constants.drivetrain.wheelTrack;

    osg::ref_ptr<osg::ShapeDrawable> outerLeftEdge = makeBox(osg::Vec3(0, widthChassis/2 - widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerLeftEdge = makeBox(osg::Vec3(0, widthChassis/2 - 1.5*widthChannel - wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> outerRightEdge = makeBox(osg::Vec3(0, -widthChassis/2 + widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerRightEdge = makeBox(osg::Vec3(0, -widthChassis/2 + 1.5*widthChannel + wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> frontEdge = makeBox(osg::Vec3(depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(frontEdge);
    osg::ref_ptr<osg::ShapeDrawable> backEdge = makeBox(osg::Vec3(-depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(backEdge);

    osg::ref_ptr<osg::ShapeDrawable> frontLeftWheel = makeCylinder(osg::Vec3(wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleLeftWheel = makeCylinder(osg::Vec3(0, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(middleLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearLeftWheel = makeCylinder(osg::Vec3(-wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = makeCylinder(osg::Vec3(wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleRightWheel = makeCylinder(osg::Vec3(0, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(middleRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearRightWheel = makeCylinder(osg::Vec3(-wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearRightWheel);

    //
    // Render the robot elevator assembly
    //

    // Bottom sprocket
    osg::ref_ptr<osg::ShapeDrawable> bottomSprocket = makeCylinder(osg::Vec3(0, 0, 0), _beltRadius, _beltWidth, Color::Orange);
    robotGeode->addDrawable(bottomSprocket);

    // Top sprocket
    osg::ref_ptr<osg::ShapeDrawable> topSprocket = makeCylinder(osg::Vec3(0, 0, _beltLength), _beltRadius, _beltWidth, Color::Orange);
    robotGeode->addDrawable(topSprocket);

    // Front and back surfaces of belt
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(_beltRadius, _beltWidth/2, 0)); // Front vertices
        vertices->push_back(osg::Vec3(_beltRadius, _beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(_beltRadius, -_beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(_beltRadius, -_beltWidth/2, 0));
        vertices->push_back(osg::Vec3(-_beltRadius, _beltWidth/2, 0)); // Back vertices
        vertices->push_back(osg::Vec3(-_beltRadius, _beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(-_beltRadius, -_beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(-_beltRadius, -_beltWidth/2, 0));
        osg::ref_ptr<osg::Geometry> belt = makeQuads(vertices, Color(Color::Orange, 127.0));
        robotGeode->addDrawable(belt);
    }
    // Outline of front and back surfaces of belt
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(_beltRadius, _beltWidth/2, 0)); // Front vertices
        vertices->push_back(osg::Vec3(_beltRadius, _beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(_beltRadius, -_beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(_beltRadius, -_beltWidth/2, 0));
        vertices->push_back(osg::Vec3(-_beltRadius, _beltWidth/2, 0)); // Back vertices
        vertices->push_back(osg::Vec3(-_beltRadius, _beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(-_beltRadius, -_beltWidth/2, _beltLength));
        vertices->push_back(osg::Vec3(-_beltRadius, -_beltWidth/2, 0));
        osg::ref_ptr<osg::Geometry> beltOutline = makeLines(vertices, Color::Orange);
        robotGeode->addDrawable(beltOutline);
    }

    // Motor shaft
    auto motorShaft = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength/2, 0), _motorShaftRadius, _motorShaftLength, Color::Gray);
    robotGeode->addDrawable(motorShaft);

    // Motor
    auto motor = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength + _motorLength/2, 0), _motorRadius, _motorLength, Color::Gray);
    robotGeode->addDrawable(motor);

    // Encoder
    auto encoder = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength - _encoderLength/2, 0), _encoderRadius, _encoderLength, Color::Green);
    robotGeode->addDrawable(encoder);

    return robotGeode;
}



osg::ref_ptr<osg::PositionAttitudeTransform> Scene::makeRobotCarriage()
{
    // Elevator carriage
    osg::ref_ptr<osg::PositionAttitudeTransform> carriagePat = new osg::PositionAttitudeTransform;
    osg::ref_ptr<osg::Geode> carriageGeode = new osg::Geode;
    carriagePat->addChild(carriageGeode);
    auto elevatorCarriage = makeBox(osg::Vec3(_beltRadius + _carriageLengthX/2, 0, 0), _carriageLengthX, _carriageLengthY, _carriageLengthZ, Color::Blue);
    carriageGeode->addDrawable(elevatorCarriage);
    return carriagePat;
}



osg::ref_ptr<osg::Geode> Scene::makeField()
{
    float width = 16.49; // 54 ft 1 in
    float depth = 8.10; // 26 ft 7 in
    float height = 0.30; // Random guess

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-width/2, -depth/2, 0));
    vertices->push_back(osg::Vec3(width/2, -depth/2, 0));
    vertices->push_back(osg::Vec3(width/2, depth/2, 0));
    vertices->push_back(osg::Vec3(-width/2, depth/2, 0));
    osg::ref_ptr<osg::Geometry> geom = makeQuads(vertices, Color(Color::Red, 32));
    geode->addDrawable(geom);

    return geode;
}



osg::ref_ptr<osg::Geometry> Scene::makeLines(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(color);

    geom->setVertexArray(vertices);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // Turn off lighting
    geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); // Turn on blending
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size())); // Geometry type is QUADS
    return geom;
}



osg::ref_ptr<osg::ShapeDrawable> Scene::makeCylinder(const osg::Vec3& pos, float radius, float height, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(pos, radius, height);
    osg::Matrix mat;
    mat.makeRotate(osg::DegreesToRadians(90.0), osg::X_AXIS);
    cylinder->setRotation(mat.getRotate());
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
    shape->setColor(color);
    return shape;
}



osg::ref_ptr<osg::ShapeDrawable> Scene::makeBox(const osg::Vec3& pos, float lengthX, float lengthY, float lengthZ, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Box> box = new osg::Box(pos, lengthX, lengthY, lengthZ);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
    shape->setColor(color);
    return shape;
}



osg::ref_ptr<osg::Geometry> Scene::makeQuads(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(color);

    geom->setVertexArray(vertices);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // Turn off lighting
    geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); // Turn on blending
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size())); // Geometry type is QUADS
    return geom;
}
