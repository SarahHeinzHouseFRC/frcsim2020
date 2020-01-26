/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "WorldModel.h"


WorldModel::WorldModel(const ConfigReader& configReader, double timestamp) :
        _collisionDetector(),
        _fieldModel(configReader, timestamp),
        _vehicleModel(configReader, timestamp),
        _gamePieceModel(configReader, 0, 0)
{

}



void WorldModel::update(double timestamp)
{
    // Update external forces on field and vehicle
    _fieldModel.update(timestamp);
    _vehicleModel.update(timestamp);

    // Apply collisions and constraints
    _collisionDetector.detectCollisions(_fieldModel, _vehicleModel, _gamePieceModel);
}
