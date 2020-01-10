/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <cmath>
#include <ConfigReader.h>
#include "RobotModel.h"


RobotModel::RobotModel(const ConfigReader& config, double startTimestamp) :
        _prevTimestamp(startTimestamp),
        _state{0},
        _leftDriveMotorMaxSpeed(config.vehicle.constants.drivetrain.motor.maxSpeed),
        _rightDriveMotorMaxSpeed(config.vehicle.constants.drivetrain.motor.maxSpeed),
        _wheelRadius(config.vehicle.constants.drivetrain.wheelRadius),
        _drivetrainWidth(config.vehicle.constants.drivetrain.width),
        _wheelTrack(config.vehicle.constants.drivetrain.wheelTrack),
        _elevatorBeltLength(config.vehicle.constants.elevator.belt.length),
        _elevatorMotorMaxSpeed(config.vehicle.constants.elevator.motor.maxSpeed),
        _elevatorMotorRadius(config.vehicle.constants.elevator.motor.radius)
{
    // Set initial state
    _state.pose.x = config.vehicle.initialState.drivetrain.x;
    _state.pose.y = config.vehicle.initialState.drivetrain.y;
    _state.elevatorMotorSpeed = config.vehicle.initialState.elevator.motorSpeed;
    _state.elevatorCarriagePos = config.vehicle.initialState.elevator.carriagePos;
}



void RobotModel::update(double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;

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
        _state.pose.x += d * cos(_state.pose.theta);
        _state.pose.y += d * sin(_state.pose.theta);
    }
    else
    {
        double r = (_wheelTrack * (vRight + vLeft)) / (2 * (vRight - vLeft));
        double dLeft = vLeft * elapsedTime;
        double dRight = vRight * elapsedTime;
        double prevTheta = _state.pose.theta;
        double currTheta = wrapAngle(prevTheta + (dRight - dLeft) / _wheelTrack);
        _state.pose.x = _state.pose.x + r*sin(currTheta) - r*sin(prevTheta);
        _state.pose.y = _state.pose.y - r*cos(currTheta) + r*cos(prevTheta);
        _state.pose.theta = currTheta;
    }

    // Update the last timestamp
    _prevTimestamp = currTimestamp;
}



void RobotModel::processCommands(const RobotCommands& commands)
{
    // Update elevator motor speed
    _state.elevatorMotorSpeed = (commands.elevatorMotorSpeed / 512.0) * _elevatorMotorMaxSpeed;

    // Update drivetrain
    _state.leftDriveMotorSpeed = (commands.leftDriveMotorSpeed / 512.0) * _leftDriveMotorMaxSpeed;
    _state.rightDriveMotorSpeed = (commands.rightDriveMotorSpeed / 512.0) * _rightDriveMotorMaxSpeed;
}



RobotState RobotModel::getState()
{
    RobotState state{0};
    state.elevatorEncoderPosition = int (1023 * _state.elevatorCarriagePos / _elevatorBeltLength);
    return state;
}
