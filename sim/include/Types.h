/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_TYPES_H
#define ROBOT_SIM_TYPES_H

#include <string>


struct SensorState
{
    int leftDriveEncoderPosition; // 0-1023
    int rightDriveEncoderPosition; // 0-1023
    int elevatorEncoderPosition; // 0-1023

    /**
     * Returns this state's information in JSON form
     */
    std::string asJson()
    {
        char tmp[1024];
        sprintf(tmp, "{ 'leftDriveEncoder': %04d, 'rightDriveEncoder': %04d, 'elevatorEncoder': %04d }", leftDriveEncoderPosition, rightDriveEncoderPosition, elevatorEncoderPosition);
        return tmp;
    }
};



struct CoreCommands
{
    int leftDriveMotorSpeed;    // -512 - 512
    int rightDriveMotorSpeed;   // -512 - 512
    int intakeCenterMotorSpeed; // -512 - 512
    int intakeLeftMotorSpeed;   // -512 - 512
    int intakeRightMotorSpeed;  // -512 - 512
    int tubeMotorSpeed;         // -512 - 512
    int timerStartStop;         // 0 or 1
    int reset;                  // 0 or 1

    /**
     * Default constructor
     */
    CoreCommands()
    {
        clear();
    };

    /**
     * Constructor from JSON string
     */
    explicit CoreCommands(const std::string& json)
    {
        leftDriveMotorSpeed = std::stoi(json.substr(25, 4));
        rightDriveMotorSpeed = std::stoi(json.substr(55, 4));
        intakeCenterMotorSpeed = std::stoi(json.substr(87, 4));
        intakeLeftMotorSpeed = std::stoi(json.substr(117, 4));
        intakeRightMotorSpeed = std::stoi(json.substr(148, 4));
        tubeMotorSpeed = std::stoi(json.substr(172, 4));
        timerStartStop = std::stoi(json.substr(196, 1));
        reset = std::stoi(json.substr(208, 1));
    }

    void clear()
    {
        leftDriveMotorSpeed = 0;
        rightDriveMotorSpeed = 0;
        intakeCenterMotorSpeed = 0;
        intakeLeftMotorSpeed = 0;
        intakeRightMotorSpeed = 0;
        tubeMotorSpeed = 0;
        timerStartStop = 0;
        reset = 0;
    }
};


#endif //ROBOT_SIM_TYPES_H
