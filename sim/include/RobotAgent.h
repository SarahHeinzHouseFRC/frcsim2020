/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef SHARP2019_ROBOTAGENT_H
#define SHARP2019_ROBOTAGENT_H


class RobotAgent
{
public:
    /**
     * Constructor
     */
    RobotAgent();

    /**
     * Sends the input encoder position to the robot
     * @param encoder Count from 0-1024
     */
    void transmitElevatorEncoderPosition(int encoder);

    /**
     * Receives elevator motor signal from the robot
     * @return Motor signal from 0-1024
     */
    double receiveElevatorMotorSignal();
};


#endif //SHARP2019_ROBOTAGENT_H
