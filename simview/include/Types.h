/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_TYPES_H
#define ROBOT_SIM_TYPES_H

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"


struct SimState
{
    struct VehicleState
    {
        int player;           // 1-6
        std::string team;     // Team number
        std::string alliance; // "Blue" or "Red"
        float x;              // Meters
        float y;              // Meters
        float theta;          // Radians
    };

    struct GamePieceState
    {
        float x; // Meters
        float y; // Meters
        float z; // Meters
    };

    int blueScore;
    int redScore;
    bool isTimerRunning;
    double timer;
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
        const Value& v = d["vehicles"];
        for (auto itr = v.Begin(); itr != v.End(); ++itr)
        {
            VehicleState vehicle{};
            vehicle.player = (*itr)["player"].GetInt();
            vehicle.team = (*itr)["team"].GetString();
            vehicle.alliance = (*itr)["alliance"].GetString();
            vehicle.x = (*itr)["x"].GetFloat();
            vehicle.y = (*itr)["y"].GetFloat();
            vehicle.theta = (*itr)["theta"].GetFloat();
            vehicles.push_back(vehicle);
        }

        const Value& g = d["gamePieces"];
        for (auto itr = g.Begin(); itr != g.End(); ++itr)
        {
            GamePieceState gamePiece{};
            gamePiece.x = (*itr)["x"].GetFloat();
            gamePiece.y = (*itr)["y"].GetFloat();
            gamePiece.z = (*itr)["z"].GetFloat();
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


#endif //ROBOT_SIM_TYPES_H
