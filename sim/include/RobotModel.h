/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_ROBOTMODEL_H
#define ROBOT_SIM_ROBOTMODEL_H

#include "ConfigReader.h"


/**
 * Models the parameters of the robot. Enforces physics-based constraints on the joints and motors of the robot.
 */
class RobotModel
{
public:
    /**
     * Constructor
     */
    RobotModel(const ConfigReader& config, double startTimestamp);

    /**
     * Updates the robot model to the new time
     * @param currTimestamp Current time (sec). This must be greater than _lastTimestamp
     * @param commandedElevatorMotorSpeed Commanded speed of elevator motor (0-1023)
     */
    void update(double currTimestamp, int commandedElevatorMotorSpeed);

    /**
     * Returns the height of the elevator carriage from the belt bottom (meters)
     */
    double getElevatorCarriagePos() const { return _elevatorCarriagePos; }

private:
    /**
     * Moves the carriage up and down based on elevator motor speed
     * @param elapsedTime Time since last call of update()
     * @param commandedElevatorMotorSpeed Commanded speed of elevator motor (0-1023)
     */
    void updateElevator(double elapsedTime, int commandedElevatorMotorSpeed);

    /**
     * Wraps the given value to be no lower than min and no higher than max
     */
    double wrap(double val, double min, double max) { if (val < min) val = min; if (val > max) val = max; return val; }

    double _elevatorBeltLength; // Need to enforce the carriage to stay bw 0 and this belt length (meters)
    double _elevatorCarriagePos; // Current height of the elevator carriage (meters)
    double _elevatorMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _elevatorMotorSpeed; // Current speed of the motor (rads/sec)
    double _elevatorMotorRadius; // Needed to calculate travel of belt per unit time
    double _prevTimestamp; // Needed to calculate how much time has passed since last update()
};


#endif //ROBOT_SIM_ROBOTMODEL_H
