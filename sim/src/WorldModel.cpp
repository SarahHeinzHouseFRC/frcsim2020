/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "WorldModel.h"


WorldModel::WorldModel(const ConfigReader& configReader, double timestamp) :
        _fieldModel(configReader, timestamp),
        _vehicleModel(configReader, timestamp),
        _gamePieceModel(configReader)
{

}



void WorldModel::update(double timestamp)
{
    // Update external forces on field and vehicle
    _fieldModel.update(timestamp);
    _vehicleModel.update(timestamp);
}
