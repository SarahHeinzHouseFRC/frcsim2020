/**
 * Copyright (c) 2020 Team 3260
 */

#include <GamePieceModel.h>
#include "WorldModel.h"


WorldModel::WorldModel(ConfigReader& configReader, int numVehicles, double timestamp) :
        _fieldModel(configReader, timestamp)
{
    // Initialize all vehicle models
    for (int i=0; i<numVehicles; i++)
    {
        VehicleModel v(configReader, timestamp, i);
        _vehicleModels.emplace_back(v);
    }

    // Initialize all game piece models
    for (const auto& initialPosition : configReader.sim.gamePiece.initialPositions)
    {
        _gamePieceModels.emplace_back(configReader.sim.gamePiece.radius, initialPosition.x, initialPosition.y);
    }

    _physicsEngine = PhysicsEngine(_fieldModel, _vehicleModels, _gamePieceModels, timestamp);
}



void WorldModel::update(double timestamp)
{
    // Update external forces on field and vehicle
    _fieldModel.update(timestamp);
    for (auto& vehicleModel : _vehicleModels)
    {
        vehicleModel.update(timestamp);
    }

    // Apply collisions and constraints
    _physicsEngine.update(_fieldModel, _vehicleModels, _gamePieceModels, timestamp);
}



void WorldModel::reset()
{
    // Field
    _fieldModel.reset();

    // Vehicle
    for (auto& vehicleModel : _vehicleModels)
    {
        vehicleModel.reset();
    }

    // Game pieces
    for (auto& gamePiece : _gamePieceModels)
    {
        gamePiece.reset();
    }

    // Physics engine
    _physicsEngine.reset(_fieldModel, _vehicleModels, _gamePieceModels);
}



SimState WorldModel::getSimState()
{
    SimState s;

    s.blueScore = std::get<0>(getScore());
    s.redScore = std::get<1>(getScore());

    for (const auto& vehicle : _vehicleModels)
    {
        SimState::VehicleState v{};
        v.player = vehicle._player;
        v.team = "3260";
        v.alliance = vehicle._alliance;
        v.x = (float) vehicle._state.pose.x;
        v.y = (float) vehicle._state.pose.y;
        v.theta = (float) vehicle._state.pose.theta;
        s.vehicles.push_back(v);
    }

    for (const auto& gamePiece : _gamePieceModels)
    {
        SimState::GamePieceState g{};
        g.x = (float) gamePiece._state.pose.x;
        g.y = (float) gamePiece._state.pose.y;
        g.z = (float) gamePiece._state.pose.z;
        s.gamePieces.push_back(g);
    }

    return s;
}
