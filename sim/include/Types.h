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
     * Deserialization from JSON string
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



struct SimState
{
    struct VehicleState
    {
        std::string team;             // Team number
        std::string alliance;         // "Blue" or "Red"
        float x;                      // Meters
        float y;                      // Meters
        float theta;                  // Radians
        float intakeCenterMotorSpeed; // Meters/sec
        float intakeLeftMotorSpeed;   // Meters/sec
        float intakeRightMotorSpeed;  // Meters/sec
        float tubeMotorSpeed;         // Meters/sec
    };

    struct FieldState
    {
        bool inCollision;
    };

    struct GamePieceState
    {
        float x; // Meters
        float y; // Meters
        float z; // Meters
        int ingestionState;
    };

    int blueScore;
    int redScore;
    bool isTimerRunning;
    double timer;
    FieldState field;
    std::vector<VehicleState> vehicles;
    std::vector<GamePieceState> gamePieces;

    /**
     * Default constructor
     */
    SimState()
    {
        clear();
    };

    /**
     * Serialization to JSON string
     */
    std::string toJson()
    {
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

        writer.StartObject();
        writer.Key("blueScore");
        writer.Int(blueScore);
        writer.Key("redScore");
        writer.Int(redScore);
        writer.Key("isTimerRunning");
        writer.Bool(isTimerRunning);
        writer.Key("timer");
        writer.Double(timer);
        writer.Key("field");
        writer.StartObject();
        writer.Key("inCollision");
        writer.Bool(field.inCollision);
        writer.EndObject();
        writer.Key("vehicles");
        writer.StartArray();
        for (const auto& v : vehicles)
        {
            writer.StartObject();
            writer.Key("team");
            writer.String(v.team.c_str());
            writer.Key("alliance");
            writer.String(v.alliance.c_str());
            writer.Key("x");
            writer.Double(v.x);
            writer.Key("y");
            writer.Double(v.y);
            writer.Key("theta");
            writer.Double(v.theta);
            writer.Key("intakeCenterMotorSpeed");
            writer.Double(v.intakeCenterMotorSpeed);
            writer.Key("intakeLeftMotorSpeed");
            writer.Double(v.intakeLeftMotorSpeed);
            writer.Key("intakeRightMotorSpeed");
            writer.Double(v.intakeRightMotorSpeed);
            writer.Key("tubeMotorSpeed");
            writer.Double(v.tubeMotorSpeed);
            writer.EndObject();
        }
        writer.EndArray();
        writer.Key("gamePieces");
        writer.StartArray();
        for (const auto& g : gamePieces)
        {
            writer.StartObject();
            writer.Key("x");
            writer.Double(g.x);
            writer.Key("y");
            writer.Double(g.y);
            writer.Key("z");
            writer.Double(g.z);
            writer.Key("ingestionState");
            writer.Int(g.ingestionState);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
        return s.GetString();
    }

    /**
     * Resets all fields to default values
     */
    void clear()
    {
        vehicles.clear();
        gamePieces.clear();
    }
};


#endif //ROBOT_SIM_TYPES_H
