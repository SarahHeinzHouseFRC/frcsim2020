/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <string>
#include "AbstractDrawer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


struct LidarPoint
{
    double azimuth, elevation, range;
};



struct SensorState
{
    float x;                             // Meters
    float y;                             // Meters
    float theta;                         // Radians
    int leftDriveEncoder;                // 0-1023
    int rightDriveEncoder;               // 0-1023
    int numIngestedBalls;                // 0+
    std::vector<LidarPoint> lidarPoints; // LIDAR points

    /**
     * Returns the sensor state as a JSON string
     */
    std::string toJson()
    {
        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);

        writer.StartObject();
        writer.Key("x");
        writer.Double(x);
        writer.Key("y");
        writer.Double(y);
        writer.Key("theta");
        writer.Double(theta);
        writer.Key("leftDriveEncoder");
        writer.Int(leftDriveEncoder);
        writer.Key("rightDriveEncoder");
        writer.Int(rightDriveEncoder);
        writer.Key("numIngestedBalls");
        writer.Int(numIngestedBalls);
        writer.Key("lidarPoints");
        writer.StartArray();
        for (const auto& p : lidarPoints)
        {
            writer.StartArray();
            writer.Double(p.azimuth);
            writer.Double(p.elevation);
            writer.Double(p.range);
            writer.EndArray();
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
        x = 0;
        y = 0;
        theta = 0;
        leftDriveEncoder = 0;
        rightDriveEncoder = 0;
        numIngestedBalls = 0;
        lidarPoints.clear();
    }
};



struct CoreCommands
{
    int leftDriveMotorSpeed;                              // -512 - 512
    int rightDriveMotorSpeed;                             // -512 - 512
    int intakeCenterMotorSpeed;                           // -512 - 512
    int intakeLeftMotorSpeed;                             // -512 - 512
    int intakeRightMotorSpeed;                            // -512 - 512
    int tubeMotorSpeed;                                   // -512 - 512
    int timerStartStop;                                   // 0 or 1
    int reset;                                            // 0 or 1
    int outtake;                                          // 0 or 1
    std::vector<std::shared_ptr<AbstractDrawer>> drawers; // Shapes to draw

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
        using namespace rapidjson;

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

        const Value& draw = d["draw"];
        for (auto itr = draw.Begin(); itr != draw.End(); itr++)
        {
            if (std::string((*itr)["shape"].GetString()) == "box")
            {
                auto text = (*itr)["text"].GetString();
                auto color = (*itr)["color"].GetString();
                auto x = (*itr)["x"].GetFloat();
                auto y = (*itr)["y"].GetFloat();
                auto width = (*itr)["width"].GetFloat();
                auto height = (*itr)["height"].GetFloat();
                std::shared_ptr<AbstractDrawer> box = std::make_shared<BoxDrawer>(text, color, x, y, width, height);
                drawers.push_back(box);
            }
            else if (std::string((*itr)["shape"].GetString()) == "line")
            {
                std::vector<std::pair<float, float>> vertices;
                const Value& v = (*itr)["vertices"];
                for (auto itr2 = v.Begin(); itr2 != v.End(); itr2++)
                {
                    float x = (*itr2)[0].GetFloat();
                    float y = (*itr2)[1].GetFloat();
                    vertices.emplace_back(x, y);
                }
                auto text = (*itr)["text"].GetString();
                auto color = (*itr)["color"].GetString();
                std::shared_ptr<AbstractDrawer> line = std::make_shared<LineDrawer>(text, color, vertices);
                drawers.push_back(line);
            }
        }
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
        drawers.clear();
    }
};



struct SimState
{
    struct VehicleState
    {
        std::string team;                    // Team number
        std::string alliance;                // "Blue" or "Red"
        float x;                             // Meters
        float y;                             // Meters
        float theta;                         // Radians
        float intakeCenterMotorSpeed;        // Meters/sec
        float intakeLeftMotorSpeed;          // Meters/sec
        float intakeRightMotorSpeed;         // Meters/sec
        float tubeMotorSpeed;                // Meters/sec
        float leftDriveMotorSpeed;           // Rads/sec
        float rightDriveMotorSpeed;          // Rads/sec
        std::vector<LidarPoint> lidarPoints; // LIDAR points
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
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);

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
            writer.Key("leftDriveMotorSpeed");
            writer.Double(v.leftDriveMotorSpeed);
            writer.Key("rightDriveMotorSpeed");
            writer.Double(v.rightDriveMotorSpeed);
            writer.Key("lidarPoints");
            writer.StartArray();
            for (const auto& p : v.lidarPoints)
            {
                writer.StartArray();
                writer.Double(p.azimuth);
                writer.Double(p.elevation);
                writer.Double(p.range);
                writer.EndArray();
            }
            writer.EndArray();
            writer.EndObject();
        }
        writer.EndArray();
        writer.Key("gamePieces");
        writer.StartArray();
        for (const auto& g : gamePieces)
        {
            writer.StartObject();
            writer.Key("position");
            writer.StartArray();
            writer.Double(g.x);
            writer.Double(g.y);
            writer.Double(g.z);
            writer.EndArray();
            writer.Key("ingestionState");
            writer.Int(g.ingestionState);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
        return s.GetString();
    }

    /**
     * Constructor from JSON string
     */
    void fromJson(const std::string& json)
    {
        using namespace rapidjson;

        clear();

        Document d;
        d.Parse(json.c_str());
        isTimerRunning = d["isTimerRunning"].GetBool();
        timer = d["timer"].GetFloat();
        blueScore = d["blueScore"].GetInt();
        redScore = d["redScore"].GetInt();

        field.inCollision = d["field"]["inCollision"].GetBool();

        const Value& v = d["vehicles"];
        for (auto itr = v.Begin(); itr != v.End(); ++itr)
        {
            VehicleState vehicle{};
            vehicle.team = (*itr)["team"].GetString();
            vehicle.alliance = (*itr)["alliance"].GetString();
            vehicle.x = (*itr)["x"].GetFloat();
            vehicle.y = (*itr)["y"].GetFloat();
            vehicle.theta = (*itr)["theta"].GetFloat();
            vehicle.intakeCenterMotorSpeed = (*itr)["intakeCenterMotorSpeed"].GetFloat();
            vehicle.intakeLeftMotorSpeed = (*itr)["intakeLeftMotorSpeed"].GetFloat();
            vehicle.intakeRightMotorSpeed = (*itr)["intakeRightMotorSpeed"].GetFloat();
            vehicle.tubeMotorSpeed = (*itr)["tubeMotorSpeed"].GetFloat();
            vehicle.leftDriveMotorSpeed = (*itr)["leftDriveMotorSpeed"].GetFloat();
            vehicle.rightDriveMotorSpeed = (*itr)["rightDriveMotorSpeed"].GetFloat();
            const Value& s = (*itr)["lidarPoints"];
            std::vector<LidarPoint> lidarPoints;
            for (auto itr2 = s.Begin(); itr2 != s.End(); itr2++)
            {
                LidarPoint p{};
                p.azimuth = (*itr2)[0].GetFloat();
                p.elevation = (*itr2)[1].GetFloat();
                p.range = (*itr2)[2].GetFloat();
                lidarPoints.push_back(p);
            }
            vehicle.lidarPoints = lidarPoints;
            vehicles.push_back(vehicle);
        }

        const Value& g = d["gamePieces"];
        for (auto itr = g.Begin(); itr != g.End(); ++itr)
        {
            GamePieceState gamePiece{};
            gamePiece.x = (*itr)["position"][0].GetFloat();
            gamePiece.y = (*itr)["position"][1].GetFloat();
            gamePiece.z = (*itr)["position"][2].GetFloat();
            gamePiece.ingestionState = (*itr)["ingestionState"].GetFloat();
            gamePieces.push_back(gamePiece);
        }
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
