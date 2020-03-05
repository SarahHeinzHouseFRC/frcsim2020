/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <osgText/Text>
#include <iostream>
#include <ConfigReader.h>
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

        addChild(drawBumpers(vehicleModel));
    }
    else
    {
        _vehicleNode = osgDB::readNodeFile(config.sim.assets.vehicleModelFile);
        addChild(_vehicleNode);
    }

    _vehicleBounds = drawCollisionBoundary(vehicleModel);
    addChild(_vehicleBounds);

    osg::ref_ptr<osg::Geode> ingestibleRegions = drawIngestibleRegions(vehicleModel);
    addChild(ingestibleRegions);

    osg::ref_ptr<osg::Geode> info = drawInfo(vehicleModel, config.sim.assets.fontFile);
    addChild(info);
}



void VehicleView::update(const VehicleModel& vehicleModel)
{
    // Update the position
    double x = vehicleModel._state.pose.x;
    double y = vehicleModel._state.pose.y;
    double theta = vehicleModel._state.pose.theta;
    this->setPosition(osg::Vec3(x, y, _wheelRadius));
    this->setAttitude(osg::Quat(theta, osg::Z_AXIS));

    // Update arrows
    {
        double percent = vehicleModel._state.intakeCenterMotorSpeed / vehicleModel._intakeCenterMotorMaxSpeed;
        osg::ref_ptr<osg::Geometry> ingestibleRegionCenterArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionCenter.center().x,
                vehicleModel._ingestibleRegionCenter.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI, 0.05 * percent, Color(Color::Orange, 127));
        osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(ingestibleRegionCenterArrow->getVertexArray());
        _ingestibleRegionCenterArrow->setVertexArray(vertices);
        _ingestibleRegionCenterArrow->getVertexArray()->dirty();
        _ingestibleRegionCenterArrow->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size()));
        _ingestibleRegionCenterArrow->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
    {
        double percent = vehicleModel._state.intakeLeftMotorSpeed / vehicleModel._intakeLeftMotorMaxSpeed;
        osg::ref_ptr<osg::Geometry> ingestibleRegionLeftArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionLeft.center().x,
                vehicleModel._ingestibleRegionLeft.center().y,
                -vehicleModel._wheelRadius + 0.1), -M_PI/2, 0.05 * percent, Color(Color::Orange, 127));
        osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>(ingestibleRegionLeftArrow->getVertexArray());
        _ingestibleRegionLeftArrow->setVertexArray(vertices);
        _ingestibleRegionLeftArrow->getVertexArray()->dirty();
        _ingestibleRegionLeftArrow->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size()));
        _ingestibleRegionLeftArrow->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
    {
        double percent = vehicleModel._state.intakeRightMotorSpeed / vehicleModel._intakeRightMotorMaxSpeed;
        osg::ref_ptr<osg::Geometry> ingestibleRegionRightArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionRight.center().x,
                vehicleModel._ingestibleRegionRight.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI/2, 0.05 * percent, Color(Color::Orange, 127));
        osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>(ingestibleRegionRightArrow->getVertexArray());
        _ingestibleRegionRightArrow->setVertexArray(vertices);
        _ingestibleRegionRightArrow->getVertexArray()->dirty();
        _ingestibleRegionRightArrow->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size()));
        _ingestibleRegionRightArrow->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
    {
        double percent = vehicleModel._state.tubeMotorSpeed / vehicleModel._tubeMotorMaxSpeed;
        osg::ref_ptr<osg::Geometry> tubeRegionArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._tubeRegion.center().x,
                vehicleModel._tubeRegion.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI, 0.05 * percent, Color(Color::Orange, 127));
        osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>(tubeRegionArrow->getVertexArray());
        _tubeRegionArrow->setVertexArray(vertices);
        _tubeRegionArrow->getVertexArray()->dirty();
        _tubeRegionArrow->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size()));
        _tubeRegionArrow->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
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

    osg::ref_ptr<osg::ShapeDrawable> outerLeftEdge = ViewUtils::drawBox(osg::Vec3(0, widthChassis/2 - widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerLeftEdge = ViewUtils::drawBox(osg::Vec3(0, widthChassis/2 - 1.5*widthChannel - wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerLeftEdge);
    osg::ref_ptr<osg::ShapeDrawable> outerRightEdge = ViewUtils::drawBox(osg::Vec3(0, -widthChassis/2 + widthChannel/2, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(outerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> innerRightEdge = ViewUtils::drawBox(osg::Vec3(0, -widthChassis/2 + 1.5*widthChannel + wheelWidth, 0), depthChassis, widthChannel, heightChannel, Color::Gray);
    robotGeode->addDrawable(innerRightEdge);
    osg::ref_ptr<osg::ShapeDrawable> frontEdge = ViewUtils::drawBox(osg::Vec3(depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(frontEdge);
    osg::ref_ptr<osg::ShapeDrawable> backEdge = ViewUtils::drawBox(osg::Vec3(-depthChassis/2, 0, 0), widthChannel, widthChassis, heightChannel, Color::Gray);
    robotGeode->addDrawable(backEdge);

    osg::ref_ptr<osg::ShapeDrawable> frontLeftWheel = ViewUtils::drawCylinder(osg::Vec3(wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearLeftWheel = ViewUtils::drawCylinder(osg::Vec3(-wheelBase/2, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = ViewUtils::drawCylinder(osg::Vec3(wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearRightWheel = ViewUtils::drawCylinder(osg::Vec3(-wheelBase/2, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    robotGeode->addDrawable(rearRightWheel);

    return robotGeode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawCollisionBoundary(const VehicleModel& vehicleModel)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        // Front left geom
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonFrontLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Rear left geom
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonRearLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Front right geom
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonFrontRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Rear right geom
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonRearRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(boundingPolygon);
    }
    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawBumpers(const VehicleModel& vehicleModel)
{
    Color color = vehicleModel._alliance == "Blue" ? Color::Blue : Color::Red;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        // Front left
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperFrontLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Front right
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperFrontRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Left
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Right
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Rear left
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperRearLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    {
        // Rear left
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._boundingPolygonBumperRearRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }
    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawIngestibleRegions(const VehicleModel& vehicleModel)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    // Center ingestible region
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionCenter.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Orange);
        geode->addDrawable(geom);
        _ingestibleRegionCenterArrow = new osg::Geometry;
        _ingestibleRegionCenterArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionCenter.center().x,
                vehicleModel._ingestibleRegionCenter.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI, 0.05, Color(Color::Orange, 127));
        geode->addDrawable(_ingestibleRegionCenterArrow);
    }

    // Left ingestible region
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionLeft.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Orange);
        geode->addDrawable(geom);
        _ingestibleRegionLeftArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionLeft.center().x,
                vehicleModel._ingestibleRegionLeft.center().y,
                -vehicleModel._wheelRadius + 0.1), -M_PI/2, 0.05, Color(Color::Orange, 127));
        geode->addDrawable(_ingestibleRegionLeftArrow);
    }

    // Right ingestible region
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._ingestibleRegionRight.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Orange);
        geode->addDrawable(geom);
        _ingestibleRegionRightArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._ingestibleRegionRight.center().x,
                vehicleModel._ingestibleRegionRight.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI/2, 0.05, Color(Color::Orange, 127));
        geode->addDrawable(_ingestibleRegionRightArrow);
    }

    // Tube region
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : vehicleModel._tubeRegion.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, -vehicleModel._wheelRadius + 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Green);
        geode->addDrawable(geom);
        _tubeRegionArrow = ViewUtils::drawArrow(osg::Vec3(
                vehicleModel._tubeRegion.center().x,
                vehicleModel._tubeRegion.center().y,
                -vehicleModel._wheelRadius + 0.1), M_PI, 0.05, Color(Color::Green, 127));
        geode->addDrawable(_tubeRegionArrow);
    }
    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawInfo(const VehicleModel& vehicleModel, const std::string& fontFile)
{
    Color color = vehicleModel._alliance == "Blue" ? Color::Blue : Color::Red;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    // Draw stalk
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0, 0, 1));
    vertices->push_back(osg::Vec3(0, 0, 2.75));
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLines(vertices, color);
    geode->addDrawable(geom);

    // Draw text
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setText("Player " + std::to_string(vehicleModel._player) + "\n" + vehicleModel._alliance + " Alliance");
    text->setFont(fontFile);
    text->setCharacterSize(0.2);
    text->setAlignment(osgText::TextBase::CENTER_BOTTOM);
    text->setAxisAlignment(osgText::TextBase::SCREEN);
    text->setPosition({ 0, 0, 3 });
    text->setColor(color);
    text->setAutoRotateToScreen(true);
    geode->addDrawable(text);

    return geode;
}
