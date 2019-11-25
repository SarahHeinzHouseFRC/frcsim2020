/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef SHARP2019_ROBOTAGENT_H
#define SHARP2019_ROBOTAGENT_H


/**
 * Receives commands and transmits robot state over UDP.
 */
class RobotAgent
{
public:
    /**
     * Constructor
     */
    RobotAgent();

    /**
     * Publishes the robot state
     */
    void txRobotState();

    /**
     * Receives robot commands
     */
    double rxRobotCommands();

    /**
     * Simulator tells the agent the encoder position to transmit (0-1024)
     */
    void setEncoderPosition(int encoderPosition) { state.elevatorEncoderPosition = encoderPosition; }

    /**
     * Simulator requests the received motor speed from the agent
     */
    int getMotorSpeed() { return commands.elevatorMotorSpeed; }

    struct
    {
        int elevatorEncoderPosition; // 0-1023
    } state;

    struct
    {
        int elevatorMotorSpeed; // 0-1023
    } commands;
};


#endif //SHARP2019_ROBOTAGENT_H
