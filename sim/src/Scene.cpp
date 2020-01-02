/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <RobotModel.h>
#include "Color.h"
#include "Scene.h"

#define IN_TO_M 0.0254


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

    _robotPat = makeRobot(config);
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
    _robotPat->setPosition(osg::Vec3(x, y, 0));
    osg::Matrix mat;
    mat.makeRotate(theta, osg::Z_AXIS);
    _robotPat->setAttitude(mat.getRotate());
}



osg::ref_ptr<osg::PositionAttitudeTransform> Scene::makeRobot(const ConfigReader& config)
{
    osg::ref_ptr<osg::PositionAttitudeTransform> robotPat = new osg::PositionAttitudeTransform;
    osg::ref_ptr<osg::Geode> geodeRobot = new osg::Geode;
    robotPat->addChild(geodeRobot);

    //
    // Render the robot drivetrain
    //

    float widthChassis = config.vehicle.constants.drivetrain.width * IN_TO_M;
    float depthChassis = config.vehicle.constants.drivetrain.depth * IN_TO_M;
    float widthChannel = config.vehicle.constants.drivetrain.widthChannel * IN_TO_M;
    float heightChannel = config.vehicle.constants.drivetrain.heightChannel * IN_TO_M;
    float wheelWidth = config.vehicle.constants.drivetrain.wheelWidth * IN_TO_M;
    float wheelRadius = config.vehicle.constants.drivetrain.wheelRadius * IN_TO_M;
    float wheelBase = config.vehicle.constants.drivetrain.wheelBase * IN_TO_M;
    float wheelTrack = config.vehicle.constants.drivetrain.wheelTrack * IN_TO_M;

    osg::ref_ptr<osg::ShapeDrawable> outerLeftEdge = makeBox(osg::Vec3(0, widthChassis/2 - widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    geodeRobot->addDrawable(outerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerLeftEdge = makeBox(osg::Vec3(0, widthChassis/2 - 1.5*widthChannel - wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    geodeRobot->addDrawable(innerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> outerRightEdge = makeBox(osg::Vec3(0, -widthChassis/2 + widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    geodeRobot->addDrawable(outerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerRightEdge = makeBox(osg::Vec3(0, -widthChassis/2 + 1.5*widthChannel + wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    geodeRobot->addDrawable(innerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> frontEdge = makeBox(osg::Vec3(depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    geodeRobot->addDrawable(frontEdge);
    osg::ref_ptr<osg::ShapeDrawable> backEdge = makeBox(osg::Vec3(-depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    geodeRobot->addDrawable(backEdge);

    float yWheel = widthChassis/2 - widthChannel - wheelWidth/2;
    osg::ref_ptr<osg::ShapeDrawable> frontLeftWheel = makeCylinder(osg::Vec3(wheelBase/2, yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(frontLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleLeftWheel = makeCylinder(osg::Vec3(0, yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(middleLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> backLeftWheel = makeCylinder(osg::Vec3(-wheelBase/2, yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(backLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = makeCylinder(osg::Vec3(wheelBase/2, -yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleRightWheel = makeCylinder(osg::Vec3(0, -yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(middleRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> backRightWheel = makeCylinder(osg::Vec3(-wheelBase/2, -yWheel, 0), wheelRadius, wheelWidth, Color::White);
    geodeRobot->addDrawable(backRightWheel);

    //
    // Render the robot elevator assembly
    //

    // Bottom sprocket
    osg::ref_ptr<osg::ShapeDrawable> bottomSprocket = makeCylinder(osg::Vec3(0, 0, 0), _beltRadius, _beltWidth, Color::Orange);
    geodeRobot->addDrawable(bottomSprocket);

    // Top sprocket
    osg::ref_ptr<osg::ShapeDrawable> topSprocket = makeCylinder(osg::Vec3(0, 0, _beltLength), _beltRadius, _beltWidth, Color::Orange);
    geodeRobot->addDrawable(topSprocket);

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
        geodeRobot->addDrawable(belt);
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
        geodeRobot->addDrawable(beltOutline);
    }

    // Motor shaft
    auto motorShaft = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength/2, 0), _motorShaftRadius, _motorShaftLength, Color::Gray);
    geodeRobot->addDrawable(motorShaft);

    // Motor
    auto motor = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength + _motorLength/2, 0), _motorRadius, _motorLength, Color::Gray);
    geodeRobot->addDrawable(motor);

    // Encoder
    auto encoder = makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength - _encoderLength/2, 0), _encoderRadius, _encoderLength, Color::Green);
    geodeRobot->addDrawable(encoder);

    return robotPat;
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
    float width = 649 * IN_TO_M; // 54 ft 1 in
    float depth = 319 * IN_TO_M; // 26 ft 7 in
    float height = 12 * IN_TO_M; // Random guess

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
