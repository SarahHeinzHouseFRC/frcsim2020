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


VehicleView::VehicleView(const ConfigReader& config, int playerId) :
        _wheelRadius(config.sim.vehicle.drivetrain.wheelRadius),
        _bodyZ(-config.sim.vehicle.drivetrain.wheelRadius + config.sim.gamePiece.radius),
        _gamePieceRadius(config.sim.gamePiece.radius),
        _centerIngestibleRegionCenter(config.sim.vehicle.ingestibleRegionCenter.center()),
        _intakeCenterMotorMaxSpeed(config.sim.vehicle.intake.centerMotorMaxSpeed),
        _centerIngestibleRegionLeft(config.sim.vehicle.ingestibleRegionLeft.center()),
        _intakeLeftMotorMaxSpeed(config.sim.vehicle.intake.leftMotorMaxSpeed),
        _centerIngestibleRegionRight(config.sim.vehicle.ingestibleRegionRight.center()),
        _intakeRightMotorMaxSpeed(config.sim.vehicle.intake.rightMotorMaxSpeed),
        _centerTubeRegion(config.sim.vehicle.tubeRegion.center()),
        _tubeMotorMaxSpeed(config.sim.vehicle.intake.tubeMotorMaxSpeed),
        _lidarNearRange(config.sim.vehicle.lidar.minRange),
        _laserFrequency(config.sim.vehicle.lidar.laserFrequency),
        _motorFrequency(config.sim.vehicle.lidar.motorFrequency)
{
    if (config.debugView || config.headless)
    {
        osg::ref_ptr<osg::Geode> wheels = drawWheels(config);
        addChild(wheels);

        osg::ref_ptr<osg::Geode> bumpers = drawBumpers(config, playerId);
        addChild(bumpers);

        _vehicleNode = drawBody(config);
        addChild(_vehicleNode);
    }
    else
    {
        std::string modelFile = config.players.at(playerId).alliance == "Blue" ? config.sim.assets.vehicleBlueModelFile : config.sim.assets.vehicleRedModelFile;
        _vehicleNode = osgDB::readNodeFile(modelFile);
        addChild(_vehicleNode);
    }

    osg::ref_ptr<osg::Geode> ingestibleRegions = drawIngestibleRegions(config);
    addChild(ingestibleRegions);

    osg::ref_ptr<osg::Geode> info = drawInfo(config, playerId, config.sim.assets.fontFile);
    addChild(info);

    _sweep = drawRays(config);
    addChild(_sweep);
}



void VehicleView::update(const SimState::VehicleState& state)
{
    // Update the position
    double x = state.x;
    double y = state.y;
    double theta = state.theta;
    this->setPosition(osg::Vec3(x, y, _wheelRadius));
    this->setAttitude(osg::Quat(theta, osg::Z_AXIS));

    // Update arrows
    std::vector<std::tuple<osg::ref_ptr<osg::Geometry>&, Geometry::Vertex2d, Color, float, float>> arrowGeometries {
            { _ingestibleRegionCenterArrow, _centerIngestibleRegionCenter, Color(Color::Orange, 127), M_PI, 0.05 * state.intakeCenterMotorSpeed / _intakeCenterMotorMaxSpeed },
            { _ingestibleRegionLeftArrow, _centerIngestibleRegionLeft, Color(Color::Orange, 127), -M_PI_2, 0.05 * state.intakeLeftMotorSpeed / _intakeLeftMotorMaxSpeed },
            { _ingestibleRegionRightArrow, _centerIngestibleRegionRight, Color(Color::Orange, 127), M_PI_2, 0.05 * state.intakeRightMotorSpeed / _intakeRightMotorMaxSpeed },
            { _tubeRegionArrow, _centerTubeRegion, Color(Color::Green, 127), M_PI, 0.05 * state.tubeMotorSpeed / _tubeMotorMaxSpeed },
    };
    for (auto& [geometry, center, color, heading, length] : arrowGeometries)
    {
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawArrow(osg::Vec3(center.x, center.y, _bodyZ), heading, length, color);
        osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
        geometry->setVertexArray(vertices);
        geometry->getVertexArray()->dirty();
        geometry->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size()));
        geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    }

    // Update LIDAR sweep
    if (!state.lidarSweep.empty())
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        double nearRange = _lidarNearRange;
        double z = -_wheelRadius + 0.089;
        for (int i=0; i<_laserFrequency/_motorFrequency; i++)
        {
            LidarPoint p = state.lidarSweep.at(i);
            if (p.range > 0)
            {
                double farRange = p.range;
                double azimuth = p.azimuth;
                osg::Vec3 nearPoint(nearRange * cos(azimuth), nearRange * sin(azimuth), z);
                osg::Vec3 farPoint(farRange * cos(azimuth), farRange * sin(azimuth), z);
                vertices->push_back(nearPoint);
                vertices->push_back(farPoint);
            }
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLines(vertices, Color::Orange);
        _sweep->removeDrawables(0);
        _sweep->addDrawable(geom);
    }
}



osg::ref_ptr<osg::Geode> VehicleView::drawWheels(const ConfigReader &config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    float wheelWidth = config.sim.vehicle.drivetrain.wheelWidth;
    float wheelRadius = config.sim.vehicle.drivetrain.wheelRadius;
    float wheelBase = config.sim.vehicle.drivetrain.wheelBase;
    float wheelTrack = config.sim.vehicle.drivetrain.wheelTrack;

    osg::ref_ptr<osg::ShapeDrawable> frontLeftWheel = ViewUtils::drawCylinder(osg::Vec3(wheelBase/2 - 0.15, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    geode->addDrawable(frontLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearLeftWheel = ViewUtils::drawCylinder(osg::Vec3(-wheelBase/2 - 0.15, wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    geode->addDrawable(rearLeftWheel);
    osg::ref_ptr<osg::ShapeDrawable> frontRightWheel = ViewUtils::drawCylinder(osg::Vec3(wheelBase/2 - 0.15, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    geode->addDrawable(frontRightWheel);
    osg::ref_ptr<osg::ShapeDrawable> rearRightWheel = ViewUtils::drawCylinder(osg::Vec3(-wheelBase/2 - 0.15, -wheelTrack/2, 0), wheelRadius, wheelWidth, Color::White);
    geode->addDrawable(rearRightWheel);

    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawBody(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    std::vector<Geometry::Polygon2d> bodyPolygons {
        config.sim.vehicle.boundingPolygonFrontLeft,
        config.sim.vehicle.boundingPolygonRearLeft,
        config.sim.vehicle.boundingPolygonFrontRight,
        config.sim.vehicle.boundingPolygonRearRight
    };

    for (const auto& polygon : bodyPolygons)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : polygon.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, _bodyZ));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(Color::Gray, 127));
        geode->addDrawable(boundingPolygon);
    }

    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawBumpers(const ConfigReader& config, int playerId)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    Color color = config.players.at(playerId).alliance == "Blue" ? Color::Blue : Color::Red;

    std::vector<Geometry::Polygon2d> bumperPolygons {
            config.sim.vehicle.boundingPolygonBumperFrontLeft,
            config.sim.vehicle.boundingPolygonBumperFrontRight,
            config.sim.vehicle.boundingPolygonBumperLeft,
            config.sim.vehicle.boundingPolygonBumperRight,
            config.sim.vehicle.boundingPolygonBumperRearLeft,
            config.sim.vehicle.boundingPolygonBumperRearRight
    };

    for (const auto& polygon : bumperPolygons)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : polygon.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, _bodyZ));
        }
        osg::ref_ptr<osg::Geometry> boundingPolygon = ViewUtils::drawQuads(vertices, Color(color, 127));
        geode->addDrawable(boundingPolygon);
    }

    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawIngestibleRegions(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    std::vector<std::tuple<Geometry::Polygon2d, Color>> ingestibleRegions {
        { config.sim.vehicle.ingestibleRegionCenter, Color::Orange },
        { config.sim.vehicle.ingestibleRegionLeft, Color::Orange },
        { config.sim.vehicle.ingestibleRegionRight, Color::Orange },
        { config.sim.vehicle.tubeRegion, Color::Green }
    };

    for (const auto& [polygon, color] : ingestibleRegions)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : polygon.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, _bodyZ));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, color);
        geode->addDrawable(geom);
    }

    std::vector<std::tuple<osg::ref_ptr<osg::Geometry>&, Geometry::Vertex2d, Color, float>> ingestibleRegionArrows {
        { _ingestibleRegionCenterArrow, config.sim.vehicle.ingestibleRegionCenter.center(), Color(Color::Orange, 127), M_PI },
        { _ingestibleRegionLeftArrow, config.sim.vehicle.ingestibleRegionLeft.center(), Color(Color::Orange, 127), -M_PI_2 },
        { _ingestibleRegionRightArrow, config.sim.vehicle.ingestibleRegionRight.center(), Color(Color::Orange, 127), M_PI_2 },
        { _tubeRegionArrow, config.sim.vehicle.tubeRegion.center(), Color(Color::Green, 127), M_PI },
    };

    for (auto& [arrowGeometry, center, color, heading] : ingestibleRegionArrows)
    {
        arrowGeometry = ViewUtils::drawArrow(osg::Vec3(center.x, center.y, _bodyZ), heading, 0, color);
        geode->addDrawable(arrowGeometry);
    }

    return geode;
}



osg::ref_ptr<osg::Geode> VehicleView::drawInfo(const ConfigReader& config, int playerId, const std::string& fontFile)
{
    Color color = config.players.at(playerId).alliance == "Blue" ? Color::Blue : Color::Red;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    // Draw stalk
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0, 0, 1));
    vertices->push_back(osg::Vec3(0, 0, 2.75));
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLines(vertices, color);
    geode->addDrawable(geom);

    // Draw text
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setText("Player " + std::to_string(playerId+1) + ": Team " + config.players.at(playerId).team);
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



osg::ref_ptr<osg::Geode> VehicleView::drawRays(const ConfigReader&config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geode->addDrawable(geom);
    return geode;
}
