/**
 * Copyright (c) 2020 Team 3260
 */

#include <cmath>
#include "VehicleModel.h"

using namespace Geometry;


VehicleModel::VehicleModel(const ConfigReader& config, double startTimestamp, int playerId) :
        _team(config.players.at(playerId).team),
        _alliance(config.players.at(playerId).alliance),
        _hasLidar(config.players.at(playerId).hasLidar),
        _leftDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.leftMotorMaxSpeed),
        _rightDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.rightMotorMaxSpeed),
        _intakeCenterMotorMaxSpeed(config.sim.vehicle.intake.centerMotorMaxSpeed),
        _intakeLeftMotorMaxSpeed(config.sim.vehicle.intake.leftMotorMaxSpeed),
        _intakeRightMotorMaxSpeed(config.sim.vehicle.intake.rightMotorMaxSpeed),
        _tubeMotorMaxSpeed(config.sim.vehicle.intake.tubeMotorMaxSpeed),
        _wheelRadius(config.sim.vehicle.drivetrain.wheelRadius),
        _wheelTrack(config.sim.vehicle.drivetrain.wheelTrack),
        _mass(config.sim.vehicle.mass),
        _boundingPolygonFrontLeft(config.sim.vehicle.boundingPolygonFrontLeft),
        _boundingPolygonFrontRight(config.sim.vehicle.boundingPolygonFrontRight),
        _boundingPolygonRearLeft(config.sim.vehicle.boundingPolygonRearLeft),
        _boundingPolygonRearRight(config.sim.vehicle.boundingPolygonRearRight),
        _boundingPolygonBumperFrontLeft(config.sim.vehicle.boundingPolygonBumperFrontLeft),
        _boundingPolygonBumperFrontRight(config.sim.vehicle.boundingPolygonBumperFrontRight),
        _boundingPolygonBumperLeft(config.sim.vehicle.boundingPolygonBumperLeft),
        _boundingPolygonBumperRight(config.sim.vehicle.boundingPolygonBumperRight),
        _boundingPolygonBumperRearLeft(config.sim.vehicle.boundingPolygonBumperRearLeft),
        _boundingPolygonBumperRearRight(config.sim.vehicle.boundingPolygonBumperRearRight),
        _ingestibleRegionCenter(config.sim.vehicle.ingestibleRegionCenter),
        _ingestibleRegionLeft(config.sim.vehicle.ingestibleRegionLeft),
        _ingestibleRegionRight(config.sim.vehicle.ingestibleRegionRight),
        _tubeRegion(config.sim.vehicle.tubeRegion),
        _initialState{ config.players.at(playerId).initialPosition.x, config.players.at(playerId).initialPosition.y, config.players.at(playerId).initialPosition.theta },
        _prevTimestamp(startTimestamp),
        _state{0},
        _outtake(false),
        _prevOuttakeButtonState(false)
{
    // Account for bad inputs
    if (_alliance != "Blue" && _alliance != "Red")
    {
        _alliance = "Red";
    }

    // Set pose
    _state.x = _initialState.x;
    _state.y = _initialState.y;
    _state.theta = _initialState.theta;
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
    double vLeft = _controls.leftDriveMotorSpeed * _wheelRadius;
    double vRight = _controls.rightDriveMotorSpeed * _wheelRadius;
    if (vLeft == vRight)
    {
        double d = vRight * elapsedTime;
        double deltaX = d * cos(_state.theta);
        double deltaY = d * sin(_state.theta);
        _state.vx = deltaX / elapsedTime;
        _state.vy = deltaY / elapsedTime;
        _state.omega = 0;
    }
    else
    {
        double r = (_wheelTrack * (vRight + vLeft)) / (2 * (vRight - vLeft));
        double dLeft = vLeft * elapsedTime;
        double dRight = vRight * elapsedTime;
        double prevTheta = _state.theta;
        double currTheta = prevTheta + (dRight - dLeft) / _wheelTrack;
        double deltaX = r * sin(currTheta) - r * sin(prevTheta);
        double deltaY = -r * cos(currTheta) + r * cos(prevTheta);
        _state.vx = deltaX / elapsedTime;
        _state.vy = deltaY / elapsedTime;
        _state.omega = (currTheta - prevTheta) / elapsedTime;
    }

    // Update the last timestamp
    _prevTimestamp = currTimestamp;
}



void VehicleModel::processCommands(const CoreCommands& commands)
{
    // Update drivetrain
    _controls.leftDriveMotorSpeed = (commands.leftDriveMotorSpeed / 512.0) * _leftDriveMotorMaxSpeed;
    _controls.rightDriveMotorSpeed = (commands.rightDriveMotorSpeed / 512.0) * _rightDriveMotorMaxSpeed;

    // Update intake motors
    _controls.intakeCenterMotorSpeed = (commands.intakeCenterMotorSpeed / 512.0) * _intakeCenterMotorMaxSpeed;
    _controls.intakeLeftMotorSpeed = (commands.intakeLeftMotorSpeed / 512.0) * _intakeLeftMotorMaxSpeed;
    _controls.intakeRightMotorSpeed = (commands.intakeRightMotorSpeed / 512.0) * _intakeRightMotorMaxSpeed;
    _controls.tubeMotorSpeed = (commands.tubeMotorSpeed / 512.0) * _tubeMotorMaxSpeed;

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
    state.x = _state.x;
    state.y = _state.y;
    state.theta = _state.theta;
    state.leftDriveEncoder = 0; // TODO: Assign real values
    state.rightDriveEncoder = 0; // TODO: Assign real values
    state.numIngestedBalls = _state.numIngestedBalls;
    state.lidarPoints = _state.lidarPoints;
    return state;
}



void VehicleModel::reset()
{
    _state = {0};
    _state.x = _initialState.x;
    _state.y = _initialState.y;
    _state.theta = _initialState.theta;
}
