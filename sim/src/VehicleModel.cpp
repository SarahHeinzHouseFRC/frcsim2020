/**
 * Copyright (c) 2020 Team 3260
 */

#include <cmath>
#include "VehicleModel.h"

using namespace Geometry;


VehicleModel::VehicleModel(const ConfigReader& config, double startTimestamp, int player) :
        _player(player),
        _team(config.players.at(_player).team),
        _alliance(config.players.at(_player).alliance),
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
        _mass(config.sim.vehicle.mass),
        _outtake(false),
        _prevOuttakeButtonState(false)
{
    // Account for bad inputs
    if (_alliance != "Blue" && _alliance != "Red")
    {
        _alliance = "Red";
    }

    // Set initial state (so we can reset to it later if needed)
    _initialState.x = config.players.at(_player).initialPosition.x;
    _initialState.y = config.players.at(_player).initialPosition.y;
    _initialState.theta = config.players.at(_player).initialPosition.theta;

    // Set pose
    _state.pose.x = _initialState.x;
    _state.pose.y = _initialState.y;
    _state.pose.theta = _initialState.theta;

    // Make bounding polygons
    _boundingPolygonFrontLeft = config.sim.vehicle.boundingPolygonFrontLeft;
    _boundingPolygonFrontRight = config.sim.vehicle.boundingPolygonFrontRight;
    _boundingPolygonRearLeft = config.sim.vehicle.boundingPolygonRearLeft;
    _boundingPolygonRearRight = config.sim.vehicle.boundingPolygonRearRight;

    _boundingPolygonBumperFrontLeft = config.sim.vehicle.boundingPolygonBumperFrontLeft;
    _boundingPolygonBumperFrontRight = config.sim.vehicle.boundingPolygonBumperFrontRight;
    _boundingPolygonBumperLeft = config.sim.vehicle.boundingPolygonBumperLeft;
    _boundingPolygonBumperRight = config.sim.vehicle.boundingPolygonBumperRight;
    _boundingPolygonBumperRearLeft = config.sim.vehicle.boundingPolygonBumperRearLeft;
    _boundingPolygonBumperRearRight = config.sim.vehicle.boundingPolygonBumperRearRight;

    // Make ingestible/ingested regions
    _ingestibleRegionCenter = config.sim.vehicle.ingestibleRegionCenter;
    _ingestibleRegionLeft = config.sim.vehicle.ingestibleRegionLeft;
    _ingestibleRegionRight = config.sim.vehicle.ingestibleRegionRight;
    _tubeRegion = config.sim.vehicle.tubeRegion;
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

    // Request the world to outtake a ball when outtake button switches from high to low
    int currOuttakeButtonState = commands.outtake;
    if (currOuttakeButtonState && !_prevOuttakeButtonState)
    {
        _outtake = true;
    }

    _prevOuttakeButtonState = currOuttakeButtonState;
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
