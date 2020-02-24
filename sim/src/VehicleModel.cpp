/**
 * Copyright (c) 2020 Team 3260
 */

#include <cmath>
#include <ConfigReader.h>
#include "VehicleModel.h"

using namespace Geometry;


VehicleModel::VehicleModel(const ConfigReader& config, double startTimestamp) :
        _prevTimestamp(startTimestamp),
        _state{0},
        _leftDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.leftMotorMaxSpeed),
        _rightDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.rightMotorMaxSpeed),
        _intakeCenterMotorMaxSpeed(config.sim.vehicle.intake.centerMotorMaxSpeed),
        _intakeLeftMotorMaxSpeed(config.sim.vehicle.intake.leftMotorMaxSpeed),
        _intakeRightMotorMaxSpeed(config.sim.vehicle.intake.rightMotorMaxSpeed),
        _tubeMotorMaxSpeed(config.sim.vehicle.intake.tubeMotorMaxSpeed),
        _wheelRadius(config.sim.vehicle.drivetrain.wheelRadius),
        _drivetrainWidth(config.sim.vehicle.drivetrain.width),
        _wheelTrack(config.sim.vehicle.drivetrain.wheelTrack),
        _mass(config.sim.vehicle.mass)
{
    // Set initial state
    _initialState = { config.sim.vehicle.initialState.x, config.sim.vehicle.initialState.y, config.sim.vehicle.initialState.theta };
    _state.pose.x = _initialState.x;
    _state.pose.y = _initialState.y;
    _state.pose.theta = _initialState.theta;

    // Make bounding polygons
    _boundingPolygonFrontLeft = std::vector<Vertex2d>{{0.20, 0.11}, {0.20, 0.31}, {0.05, 0.31}, {0.05, 0.11}};
    _boundingPolygonFrontRight = std::vector<Vertex2d>{{0.20, -0.31}, {0.20, -0.11}, {0.05, -0.11}, {0.05, -0.31}};
    _boundingPolygonRearLeft = std::vector<Vertex2d>{{0.05, 0.18}, {0.05, 0.31}, {-0.51, 0.31}, {-0.51, 0.18}};
    _boundingPolygonRearRight = std::vector<Vertex2d>{{0.05, -0.31}, {0.05, -0.18}, {-0.51, -0.18}, {-0.51, -0.31}};

    _boundingPolygonBumperFrontLeft = std::vector<Vertex2d>{ {0.30, 0.16}, {0.30, 0.31}, {0.20, 0.31}, {0.20, 0.16} };
    _boundingPolygonBumperFrontRight = std::vector<Vertex2d>{ {0.30, -0.31}, {0.30, -0.16}, {0.20, -0.16}, {0.20, -0.31} };
    _boundingPolygonBumperLeft = std::vector<Vertex2d>{ {0.30, 0.31}, {0.30, 0.40}, {-0.60, 0.40}, {-0.60, 0.31} };
    _boundingPolygonBumperRight = std::vector<Vertex2d>{ {0.30, -0.40}, {0.30, -0.31}, {-0.60, -0.31}, {-0.60, -0.40} };
    _boundingPolygonBumperRearLeft = std::vector<Vertex2d>{ {-0.51, 0.18}, {-0.51, 0.31}, {-0.60, 0.31}, {-0.60, 0.18} };
    _boundingPolygonBumperRearRight = std::vector<Vertex2d>{ {-0.51, -0.31}, {-0.51, -0.18}, {-0.60, -0.18}, {-0.60, -0.31} };

    // Make ingestible/ingested regions
    _ingestibleRegionCenter = std::vector<Vertex2d>{{0.51, -0.11}, {0.51, 0.11}, {0.05, 0.11}, {0.05, -0.11}};
    _ingestibleRegionLeft = std::vector<Vertex2d>{{0.51, 0.11}, {0.51, 0.26}, {0.30, 0.26}, {0.30, 0.11}};
    _ingestibleRegionRight = std::vector<Vertex2d>{{0.51, -0.26}, {0.51, -0.11}, {0.30, -0.11}, {0.30, -0.26}};
    _tubeRegion = std::vector<Vertex2d>{{0.05, -0.18}, {0.05, 0.18}, {-0.60, 0.18}, {-0.60, -0.18}};
}



void VehicleModel::update(double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;
    if (elapsedTime == 0)
    {
        return; // Nothing to update
    }

    //
    // Update pose based on drivetrain motor speeds
    //

    // Increment pose
    double vLeft = _state.leftDriveMotorSpeed * _wheelRadius;
    double vRight = _state.rightDriveMotorSpeed * _wheelRadius;
    if (vLeft == vRight)
    {
        double d = vRight * elapsedTime;
        double deltaX = d * cos(_state.pose.theta);
        double deltaY = d * sin(_state.pose.theta);
        _state.pose.vx = deltaX / elapsedTime;
        _state.pose.vy = deltaY / elapsedTime;
        _state.pose.omega = 0;
    }
    else
    {
        double r = (_wheelTrack * (vRight + vLeft)) / (2 * (vRight - vLeft));
        double dLeft = vLeft * elapsedTime;
        double dRight = vRight * elapsedTime;
        double prevTheta = _state.pose.theta;
        double currTheta = prevTheta + (dRight - dLeft) / _wheelTrack;
        double deltaX = r * sin(currTheta) - r * sin(prevTheta);
        double deltaY = -r * cos(currTheta) + r * cos(prevTheta);
        _state.pose.vx = deltaX / elapsedTime;
        _state.pose.vy = deltaY / elapsedTime;
        _state.pose.omega = (currTheta - prevTheta) / elapsedTime;
    }

    // Update the last timestamp
    _prevTimestamp = currTimestamp;
}



void VehicleModel::processCommands(const CoreCommands& commands)
{
    // Update drivetrain
    _state.leftDriveMotorSpeed = (commands.leftDriveMotorSpeed / 512.0) * _leftDriveMotorMaxSpeed;
    _state.rightDriveMotorSpeed = (commands.rightDriveMotorSpeed / 512.0) * _rightDriveMotorMaxSpeed;

    // Update intake motors
    _state.intakeCenterMotorSpeed = (commands.intakeCenterMotorSpeed / 512.0) * _intakeCenterMotorMaxSpeed;
    _state.intakeLeftMotorSpeed = (commands.intakeLeftMotorSpeed / 512.0) * _intakeLeftMotorMaxSpeed;
    _state.intakeRightMotorSpeed = (commands.intakeRightMotorSpeed / 512.0) * _intakeRightMotorMaxSpeed;
    _state.tubeMotorSpeed = (commands.tubeMotorSpeed / 512.0) * _tubeMotorMaxSpeed;
}



SensorState VehicleModel::getSensorState()
{
    SensorState state{0};
    return state;
}



void VehicleModel::reset()
{
    _state = {0};
    _state.pose.x = _initialState.x;
    _state.pose.y = _initialState.y;
    _state.pose.theta = _initialState.theta;
}
