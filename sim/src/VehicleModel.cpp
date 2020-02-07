/**
 * Copyright (c) 2020 Team 3260
 */

#include <cmath>
#include <ConfigReader.h>
#include <VehicleModel.h>
#include "VehicleModel.h"

using namespace Geometry;


VehicleModel::VehicleModel(const ConfigReader& config, double startTimestamp) :
        _prevTimestamp(startTimestamp),
        _state{0},
        _leftDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.motor.maxSpeed),
        _rightDriveMotorMaxSpeed(config.sim.vehicle.drivetrain.motor.maxSpeed),
        _wheelRadius(config.sim.vehicle.drivetrain.wheelRadius),
        _drivetrainWidth(config.sim.vehicle.drivetrain.width),
        _wheelTrack(config.sim.vehicle.drivetrain.wheelTrack),
        _elevatorBeltLength(config.sim.vehicle.elevator.belt.length),
        _elevatorMotorMaxSpeed(config.sim.vehicle.elevator.motor.maxSpeed),
        _elevatorMotorRadius(config.sim.vehicle.elevator.motor.radius),
        _mass(config.sim.vehicle.mass)
{
    // Set initial state
    _state.pose.x = config.sim.vehicle.initialState.x;
    _state.pose.y = config.sim.vehicle.initialState.y;
    _state.pose.theta = config.sim.vehicle.initialState.theta;
    _state.elevatorMotorSpeed = config.sim.vehicle.elevator.initialState.motorSpeed;
    _state.elevatorCarriagePos = config.sim.vehicle.elevator.initialState.carriagePos;

    // Make bounding polygon
    _boundingPolygon = Polygon2d(config.sim.vehicle.polygon);
    _boundingPolygonWorld = _boundingPolygon.transform(_state.pose.x, _state.pose.y, _state.pose.theta);
}



void VehicleModel::update(double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;
    if (elapsedTime == 0)
    {
        return; // Nothing to update
    }

    //
    // Update elevator position based on elevator motor speed
    //

    // Move the carriage up by y = omega * r * t
    double y = _state.elevatorMotorSpeed * _elevatorMotorRadius * elapsedTime;
    _state.elevatorCarriagePos += y;
    _state.elevatorCarriagePos = bound(_state.elevatorCarriagePos, 0, _elevatorBeltLength);

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

    _boundingPolygonWorld = _boundingPolygon.transform(_state.pose.x, _state.pose.y, _state.pose.theta);

    // Update the last timestamp
    _prevTimestamp = currTimestamp;
}



void VehicleModel::processCommands(const CoreCommands& commands)
{
    // Update elevator motor speed
    _state.elevatorMotorSpeed = (commands.elevatorMotorSpeed / 512.0) * _elevatorMotorMaxSpeed;

    // Update drivetrain
    _state.leftDriveMotorSpeed = (commands.leftDriveMotorSpeed / 512.0) * _leftDriveMotorMaxSpeed;
    _state.rightDriveMotorSpeed = (commands.rightDriveMotorSpeed / 512.0) * _rightDriveMotorMaxSpeed;
}



SensorState VehicleModel::getSensorState()
{
    SensorState state{0};
    state.elevatorEncoderPosition = int (1023 * _state.elevatorCarriagePos / _elevatorBeltLength);
    return state;
}
