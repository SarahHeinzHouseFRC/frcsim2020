/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_WORLDMODEL_H
#define ROBOT_SIM_WORLDMODEL_H

#include <vector>
#include "ConfigReader.h"
#include "FieldModel.h"
#include "VehicleModel.h"
#include "GamePieceModel.h"
#include "PhysicsEngine.h"


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

    /**
     * Resets the field
     */
    void reset();

    /**
     * Getter for vehicle model
     */
    VehicleModel& vehicleModel() { return _vehicleModel; }

    /**
     * Getter for field model
     */
    FieldModel& fieldModel() { return _fieldModel; }

    /**
     * Returns the current score
     */
    std::tuple<int, int> getScore() { return _fieldModel.getScore(); }

private:
    FieldModel _fieldModel;
    VehicleModel _vehicleModel;
    PhysicsEngine _physicsEngine;
    std::vector<GamePieceModel> _gamePieceModels;
};


#endif //ROBOT_SIM_WORLDMODEL_H
