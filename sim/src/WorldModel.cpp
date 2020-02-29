/**
 * Copyright (c) 2020 Team 3260
 */

#include "WorldModel.h"


WorldModel::WorldModel(const ConfigReader& configReader, double timestamp) :
        _fieldModel(configReader, timestamp),
        _vehicleModel(configReader, timestamp)
{
    // Initialize all game pieces
    for (const auto& initialPosition : configReader.sim.gamePiece.initialPositions)
    {
        _gamePieceModels.emplace_back(configReader.sim.gamePiece.radius, initialPosition.x, initialPosition.y);
    }

    _physicsEngine = PhysicsEngine(_fieldModel, _vehicleModel, _gamePieceModels, timestamp);
}



void WorldModel::update(double timestamp)
{
    // Update external forces on field and vehicle
    _fieldModel.update(timestamp);
    _vehicleModel.update(timestamp);

    // Apply collisions and constraints
    _physicsEngine.update(_fieldModel, _vehicleModel, _gamePieceModels, timestamp);
}



void WorldModel::reset()
{
    // Field
    _fieldModel.reset();

    // Vehicle
    _vehicleModel.reset();

    // Game pieces
    for (auto& gamePiece : _gamePieceModels)
    {
        gamePiece.reset();
    }

    // Physics engine
    _physicsEngine.reset(_fieldModel, _vehicleModel, _gamePieceModels);
}
