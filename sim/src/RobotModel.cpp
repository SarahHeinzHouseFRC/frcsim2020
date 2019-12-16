/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <ConfigReader.h>
#include "RobotModel.h"

#define RPM_TO_RADS_PER_SEC 0.104719755


RobotModel::RobotModel(const ConfigReader& config, double startTimestamp) :
        _elevatorBeltLength(config.vehicle.constants.elevator.belt.length),
        _elevatorMotorMaxSpeed(config.vehicle.constants.elevator.motor.maxSpeed*RPM_TO_RADS_PER_SEC),
        _elevatorMotorRadius(config.vehicle.constants.elevator.motor.radius),
        _state{0},
        _prevTimestamp(startTimestamp)
{
    _state.elevatorMotorSpeed = config.vehicle.initialState.elevator.motorSpeed;
    _state.elevatorCarriagePos = config.vehicle.initialState.elevator.carriagePos;
}



void RobotModel::update(double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;

    // Update the elevator
    updateElevator(elapsedTime);

    // Update the last timestamp
    _prevTimestamp = currTimestamp;
}



void RobotModel::processCommands(const RobotCommands& commands)
{
    // Update elevator motor speed
    _state.elevatorMotorSpeed = (commands.elevatorMotorSpeed / 512.0) * _elevatorMotorMaxSpeed;
}



RobotState RobotModel::getState()
{
    RobotState state{0};
    state.elevatorEncoderPosition = int (1023 * _state.elevatorCarriagePos / _elevatorBeltLength);
    return state;
}



void RobotModel::updateElevator(double elapsedTime)
{
    // Move the carriage up by d = omega * r * t
    double d = _state.elevatorMotorSpeed * _elevatorMotorRadius * elapsedTime;
    _state.elevatorCarriagePos += d;
    _state.elevatorCarriagePos = wrap(_state.elevatorCarriagePos, 0, _elevatorBeltLength);
}
