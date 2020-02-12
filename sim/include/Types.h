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
    int leftDriveMotorSpeed;  // -511 - 512
    int rightDriveMotorSpeed; // -511 - 512
    int elevatorMotorSpeed;   // -511 - 512
    int back;                 // 0 or 1
    int guide;                // 0 or 1
    int start;                // 0 or 1

    /**
     * Default constructor
     */
    CoreCommands()
    {
        reset();
    };

    /**
     * Constructor from JSON string
     */
    explicit CoreCommands(const std::string& json)
    {
        leftDriveMotorSpeed = std::stoi(json.substr(25, 4));
        rightDriveMotorSpeed = std::stoi(json.substr(55, 4));
        elevatorMotorSpeed = std::stoi(json.substr(83, 4));
        back = std::stoi(json.substr(97, 1));
        guide = std::stoi(json.substr(109, 1));
        start = std::stoi(json.substr(121, 1));
    }

    void reset()
    {
        leftDriveMotorSpeed = 0;
        rightDriveMotorSpeed = 0;
        elevatorMotorSpeed = 0;
        back = 0;
        guide = 0;
        start = 0;
    }
};


#endif //ROBOT_SIM_TYPES_H
