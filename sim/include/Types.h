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
        sprintf(tmp, "{ %05d %05d %05d }", leftDriveEncoderPosition, rightDriveEncoderPosition, elevatorEncoderPosition);
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
        leftDriveMotorSpeed = std::stoi(json.substr(2, 7));
        rightDriveMotorSpeed = std::stoi(json.substr(8, 13));
        elevatorMotorSpeed = std::stoi(json.substr(14, 19));
        back = std::stoi(json.substr(20, 1));
        guide = std::stoi(json.substr(21, 1));
        start = std::stoi(json.substr(22, 1));
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
