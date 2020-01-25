/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgDB/ReadFile>
#include <iostream>
#include "VehicleView.h"
#include "ViewUtils.h"
#include "Color.h"

#define DEFAULT_VEHICLE_FILE "/home/psahay/chassis.wrl"


VehicleView::VehicleView(const ConfigReader& config, const VehicleModel& vehicleModel) :
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
    if (config.debugView)
    {
        _vehicleNode = makeVehicle(config);
        addChild(_vehicleNode);

        _elevatorPat = makeVehicleElevator();
        addChild(_elevatorPat);
    }
    else
    {
        _vehicleNode = osgDB::readNodeFile(DEFAULT_VEHICLE_FILE);
        addChild(_vehicleNode);
    }
}



void VehicleView::update(const VehicleModel& vehicleModel)
{
    // Update elevator position
    if (_elevatorPat)
    {
        double elevatorPos = vehicleModel._state.elevatorCarriagePos;
        _elevatorPat->setPosition(osg::Vec3(0, 0, elevatorPos));
    }

    // Update the position
    double x = vehicleModel._state.pose.x;
    double y = vehicleModel._state.pose.y;
    double theta = vehicleModel._state.pose.theta;
    this->setPosition(osg::Vec3(x, y, _wheelRadius));
    osg::Matrix mat;
    mat.makeRotate(theta, osg::Z_AXIS);
    this->setAttitude(mat.getRotate());
}



osg::ref_ptr<osg::Geode> VehicleView::makeVehicle(const ConfigReader &config)
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

    osg::ref_ptr<osg::ShapeDrawable> outerLeftEdge = ViewUtils::makeBox(osg::Vec3(0, widthChassis/2 - widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerLeftEdge = ViewUtils::makeBox(osg::Vec3(0, widthChassis/2 - 1.5*widthChannel - wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> outerRightEdge = ViewUtils::makeBox(osg::Vec3(0, -widthChassis/2 + widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerRightEdge = ViewUtils::makeBox(osg::Vec3(0, -widthChassis/2 + 1.5*widthChannel + wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> frontEdge = ViewUtils::makeBox(osg::Vec3(depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(frontEdge);
    osg::ref_ptr<osg::ShapeDrawable> backEdge = ViewUtils::makeBox(osg::Vec3(-depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(backEdge);

    osg::ref_ptr<osg::ShapeDrawable> frontLeftWheel = ViewUtils::makeCylinder(osg::Vec3(wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleLeftWheel = ViewUtils::makeCylinder(osg::Vec3(0, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(middleLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearLeftWheel = ViewUtils::makeCylinder(osg::Vec3(-wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = ViewUtils::makeCylinder(osg::Vec3(wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> middleRightWheel = ViewUtils::makeCylinder(osg::Vec3(0, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(middleRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearRightWheel = ViewUtils::makeCylinder(osg::Vec3(-wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearRightWheel);

    //
    // Render the robot elevator assembly
    //

    // Bottom sprocket
    osg::ref_ptr<osg::ShapeDrawable> bottomSprocket = ViewUtils::makeCylinder(osg::Vec3(0, 0, 0), _beltRadius, _beltWidth, Color::Orange);
    robotGeode->addDrawable(bottomSprocket);

    // Top sprocket
    osg::ref_ptr<osg::ShapeDrawable> topSprocket = ViewUtils::makeCylinder(osg::Vec3(0, 0, _beltLength), _beltRadius, _beltWidth, Color::Orange);
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
        osg::ref_ptr<osg::Geometry> belt = ViewUtils::makeQuads(vertices, Color(Color::Orange, 127.0));
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
        osg::ref_ptr<osg::Geometry> beltOutline = ViewUtils::makeLines(vertices, Color::Orange);
        robotGeode->addDrawable(beltOutline);
    }

    // Motor shaft
    auto motorShaft = ViewUtils::makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength/2, 0), _motorShaftRadius, _motorShaftLength, Color::Gray);
    robotGeode->addDrawable(motorShaft);

    // Motor
    auto motor = ViewUtils::makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength + _motorLength/2, 0), _motorRadius, _motorLength, Color::Gray);
    robotGeode->addDrawable(motor);

    // Encoder
    auto encoder = ViewUtils::makeCylinder(osg::Vec3(0, _beltWidth/2 + _motorShaftLength - _encoderLength/2, 0), _encoderRadius, _encoderLength, Color::Green);
    robotGeode->addDrawable(encoder);

    return robotGeode;
}



osg::ref_ptr<osg::PositionAttitudeTransform> VehicleView::makeVehicleElevator()
{
    // Elevator carriage
    osg::ref_ptr<osg::PositionAttitudeTransform> carriagePat = new osg::PositionAttitudeTransform;
    osg::ref_ptr<osg::Geode> carriageGeode = new osg::Geode;
    carriagePat->addChild(carriageGeode);
    auto elevatorCarriage = ViewUtils::makeBox(osg::Vec3(_beltRadius + _carriageLengthX/2, 0, 0), _carriageLengthX, _carriageLengthY, _carriageLengthZ, Color::Blue);
    carriageGeode->addDrawable(elevatorCarriage);
    return carriagePat;
}
