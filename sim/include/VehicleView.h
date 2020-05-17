/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "Types.h"


class VehicleView : public osg::PositionAttitudeTransform
{
public:
    /**
     * Constructor
     */
    VehicleView(const ConfigReader& config, int playerId);

    /**
     * Updates the vehicle view based on the vehicle model
     */
    void update(const SimState::VehicleState& state, bool showLidar);

    /**
     * Returns vehicle node (for Visualizer to center view on)
     */
    osg::ref_ptr<osg::Node> getVehicleNode() { return _vehicleNode; }

private:
    /**
     * Draws wheels
     */
    osg::ref_ptr<osg::Geode> drawWheels(const ConfigReader& config);

    /**
     * Draws the collision boundary of the vehicle
     */
    osg::ref_ptr<osg::Geode> drawBody(const ConfigReader& config);

    /**
     * Draws the bumpers around the vehicle
     */
    osg::ref_ptr<osg::Geode> drawBumpers(const ConfigReader& config, int playerId);

    /**
     * Draws the ingestible region of the vehicle
     */
    osg::ref_ptr<osg::Geode> drawIngestibleRegions(const ConfigReader& config);

    /**
     * Draws info about the vehicle, player number, etc.
     */
    osg::ref_ptr<osg::Geode> drawInfo(const ConfigReader& config, int playerId, const std::string& fontFile);

    /**
     * Draws LIDAR rays
     */
    osg::ref_ptr<osg::Geode> drawRays(const ConfigReader&config);

    osg::ref_ptr<osg::Node> _vehicleNode;
    osg::ref_ptr<osg::Geode> _sweep;
    osg::ref_ptr<osg::Geometry> _ingestibleRegionCenterArrow;
    Geometry::Vertex2d _centerIngestibleRegionCenter;
    Geometry::Vertex2d _centerIngestibleRegionLeft;
    Geometry::Vertex2d _centerIngestibleRegionRight;
    Geometry::Vertex2d _centerTubeRegion;
    float _intakeCenterMotorMaxSpeed;
    float _intakeLeftMotorMaxSpeed;
    float _intakeRightMotorMaxSpeed;
    float _tubeMotorMaxSpeed;
    osg::ref_ptr<osg::Geometry> _ingestibleRegionLeftArrow;
    osg::ref_ptr<osg::Geometry> _ingestibleRegionRightArrow;
    osg::ref_ptr<osg::Geometry> _tubeRegionArrow;
    float _wheelRadius;
    float _lidarNearRange;
    float _laserFrequency;
    float _motorFrequency;
    float _gamePieceRadius;
    float _bodyZ;
};
