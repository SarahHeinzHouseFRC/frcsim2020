/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "WorldModel.h"


WorldModel::WorldModel(const ConfigReader& configReader, double timestamp) :
        _fieldModel(configReader, timestamp),
        _vehicleModel(configReader, timestamp)
{
    // Top 5
    _gamePieceModels.push_back({ configReader, 1.20, 1.63 });
    _gamePieceModels.push_back({ configReader, 1.04, 2.02 });
    _gamePieceModels.push_back({ configReader, 0.42, 2.20 });
    _gamePieceModels.push_back({ configReader, 0.03, 2.04 });
    _gamePieceModels.push_back({ configReader, -0.36, 1.88 });

    // Bottom 5
    _gamePieceModels.push_back({ configReader, -1.20, -1.63 });
    _gamePieceModels.push_back({ configReader, -1.04, -2.02 });
    _gamePieceModels.push_back({ configReader, -0.42, -2.20 });
    _gamePieceModels.push_back({ configReader, -0.03, -2.04 });
    _gamePieceModels.push_back({ configReader, 0.36, -1.88 });

    // Right 5
    _gamePieceModels.push_back({ configReader, 3.63, -1.63 });
    _gamePieceModels.push_back({ configReader, 3.16, -1.63 });
    _gamePieceModels.push_back({ configReader, 3.40, 0.00 });
    _gamePieceModels.push_back({ configReader, 3.40, 0.91 });
    _gamePieceModels.push_back({ configReader, 3.40, 1.83 });

    // Left 5
    _gamePieceModels.push_back({ configReader, -3.63, 1.63 });
    _gamePieceModels.push_back({ configReader, -3.16, 1.63 });
    _gamePieceModels.push_back({ configReader, -3.40, -0.00 });
    _gamePieceModels.push_back({ configReader, -3.40, -0.91 });
    _gamePieceModels.push_back({ configReader, -3.40, -1.83 });

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
}
