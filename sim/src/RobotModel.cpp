/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <ConfigReader.h>
#include "RobotModel.h"

#define RPM_TO_RADS_PER_SEC 0.104719755


RobotModel::RobotModel(const ConfigReader& config, double startTimestamp) :
        _elevatorBeltLength(config.elevator.belt.length),
        _elevatorCarriagePos(0.15),
        _elevatorMotorMaxSpeed(config.elevator.motor.maxSpeed*RPM_TO_RADS_PER_SEC),
        _elevatorMotorRadius(config.elevator.motor.radius),
        _elevatorMotorSpeed(0),
        _prevTimestamp(startTimestamp)
{

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
    _elevatorMotorSpeed = commands.elevatorMotorSpeed / 256.0;
}



RobotState RobotModel::getState()
{
    RobotState state;
    state.elevatorEncoderPosition = int (1024 * _elevatorCarriagePos / _elevatorBeltLength);
    return state;
}



void RobotModel::updateElevator(double elapsedTime)
{
    // Move the carriage up by d = omega * r * t
    double d = _elevatorMotorSpeed * _elevatorMotorRadius * elapsedTime;
    _elevatorCarriagePos += d;
    _elevatorCarriagePos = wrap(_elevatorCarriagePos, 0, _elevatorBeltLength);
}
