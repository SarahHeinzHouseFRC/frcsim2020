/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_WORLDMODEL_H
#define ROBOT_SIM_WORLDMODEL_H

#include "ConfigReader.h"
#include "FieldModel.h"
#include "VehicleModel.h"
#include "GamePieceModel.h"
#include "CollisionDetector.h"


class WorldModel
{
friend class Scene;
public:
    /**
     * Constructor
     */
    WorldModel(const ConfigReader& configReader, double timestamp);

    /**
     * Updates the field, vehicle, and game piece models
     */
    void update(double timestamp);

    VehicleModel& vehicleModel() { return _vehicleModel; }

private:
    CollisionDetector _collisionDetector;
    FieldModel _fieldModel;
    VehicleModel _vehicleModel;
    GamePieceModel _gamePieceModel;
};


#endif //ROBOT_SIM_WORLDMODEL_H
