/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <iostream>
#include "VehicleView.h"
#include "ViewUtils.h"
#include "Color.h"


VehicleView::VehicleView(const ConfigReader& config, const VehicleModel& vehicleModel) :
        _wheelRadius(config.sim.vehicle.drivetrain.wheelRadius)
{
    if (config.debugView)
    {
        _vehicleNode = drawVehicle(config);
        addChild(_vehicleNode);
    }
    else
    {
        _vehicleNode = osgDB::readNodeFile(config.sim.assets.vehicleModelFile);
        addChild(_vehicleNode);
    }

    _vehicleBounds = drawCollisionBoundary(vehicleModel);
    addChild(_vehicleBounds);

    osg::ref_ptr<osg::Geode> ingestibleRegion = drawIngestibleRegion(vehicleModel);
    addChild(ingestibleRegion);
}



void VehicleView::update(const VehicleModel& vehicleModel)
{
    // Update the position
    double x = vehicleModel._state.pose.x;
    double y = vehicleModel._state.pose.y;
    double theta = vehicleModel._state.pose.theta;
    this->setPosition(osg::Vec3(x, y, _wheelRadius));
    osg::Matrix mat;
    mat.makeRotate(theta, osg::Z_AXIS);
    this->setAttitude(mat.getRotate());
}



osg::ref_ptr<osg::Geode> VehicleView::drawVehicle(const ConfigReader &config)
{
    osg::ref_ptr<osg::Geode> robotGeode = new osg::Geode;

    //
    // Render the robot drivetrain
    //

    float widthChassis = config.sim.vehicle.drivetrain.width;
    float depthChassis = config.sim.vehicle.drivetrain.depth;
    float widthChannel = config.sim.vehicle.drivetrain.widthChannel;
    float heightChannel = config.sim.vehicle.drivetrain.heightChannel;
    float wheelWidth = config.sim.vehicle.drivetrain.wheelWidth;
    float wheelRadius = config.sim.vehicle.drivetrain.wheelRadius;
    float wheelBase = config.sim.vehicle.drivetrain.wheelBase;
    float wheelTrack = config.sim.vehicle.drivetrain.wheelTrack;

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
    osg::ref_ptr<osg::ShapeDrawable> rearLeftWheel = ViewUtils::makeCylinder(osg::Vec3(-wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = ViewUtils::makeCylinder(osg::Vec3(wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearRightWheel = ViewUtils::makeCylinder(osg::Vec3(-wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearRightWheel);

    return robotGeode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawCollisionBoundary(const VehicleModel& vehicleModel)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::makeLineLoop(vertices, Color::Green);
        geode->addDrawable(boundingPolygon);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::makeLineLoop(vertices, Color::Green);
        geode->addDrawable(boundingPolygon);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonRear.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::makeLineLoop(vertices, Color::Green);
        geode->addDrawable(boundingPolygon);
    }
    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawIngestibleRegion(const VehicleModel& vehicleModel)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionCenter.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, Color::Orange);
        geode->addDrawable(geom);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, Color::Blue);
        geode->addDrawable(geom);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, Color::Red);
        geode->addDrawable(geom);
    }
    return geode;
}
