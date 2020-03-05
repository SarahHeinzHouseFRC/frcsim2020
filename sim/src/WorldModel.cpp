/**
 * Copyright (c) 2020 Team 3260
 */

#include "WorldModel.h"


WorldModel::WorldModel(ConfigReader& configReader, int numVehicles, double timestamp) :
        _fieldModel(configReader, timestamp)
{
    // Initialize all vehicle models
    for (int i=0; i<numVehicles; i++)
    {
        configReader.sim.vehicle.initialState.y += 1.5;
        _vehicleModels.emplace_back(configReader, timestamp, i+1, (i % 2 == 0) ? "Blue" : "Red");
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
