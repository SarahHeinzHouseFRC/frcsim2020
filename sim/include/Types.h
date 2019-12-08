/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_TYPES_H
#define ROBOT_SIM_TYPES_H


struct RobotState
{
    int elevatorEncoderPosition; // 0-1023
};



struct RobotCommands
{
    int elevatorMotorSpeed; // 0-1023
};


#endif //ROBOT_SIM_TYPES_H
