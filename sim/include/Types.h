/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_TYPES_H
#define ROBOT_SIM_TYPES_H

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"


struct SensorState
{
    int leftDriveEncoder;  // 0-1023
    int rightDriveEncoder; // 0-1023
    int elevatorEncoder;   // 0-1023

    /**
     * Returns the sensor state as a JSON string
     */
    std::string toJson()
    {
        const char msg[] = R"({ "leftDriveEncoder": 0, "rightDriveEncoder": 0, "elevatorEncoder": 0 })";
        rapidjson::Document d;
        d.Parse(msg);
        d["leftDriveEncoder"] = leftDriveEncoder;
        d["rightDriveEncoder"] = rightDriveEncoder;
        d["elevatorEncoder"] = elevatorEncoder;

        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        d.Accept(writer);
        return sb.GetString();
    }

    /**
     * Resets all fields to default values
     */
    void clear()
    {
        leftDriveEncoder = 0;
        rightDriveEncoder = 0;
        elevatorEncoder = 0;
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
    int outtake;                // 0 or 1

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
    void fromJson(const std::string& json)
    {
        rapidjson::Document d;
        d.Parse(json.c_str());
        leftDriveMotorSpeed    = d["leftDriveMotorSpeed"].GetInt();
        rightDriveMotorSpeed   = d["rightDriveMotorSpeed"].GetInt();
        intakeCenterMotorSpeed = d["intakeCenterMotorSpeed"].GetInt();
        intakeLeftMotorSpeed   = d["intakeLeftMotorSpeed"].GetInt();
        intakeRightMotorSpeed  = d["intakeRightMotorSpeed"].GetInt();
        tubeMotorSpeed         = d["tubeMotorSpeed"].GetInt();
        timerStartStop         = d["timerStartStop"].GetInt();
        reset                  = d["reset"].GetInt();
        outtake                = d["outtake"].GetInt();
    }

    /**
     * Resets all fields to default values
     */
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
        outtake = 0;
    }
};


#endif //ROBOT_SIM_TYPES_H
