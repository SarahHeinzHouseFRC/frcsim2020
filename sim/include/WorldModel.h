/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <vector>
#include "ConfigReader.h"
#include "FieldModel.h"
#include "VehicleModel.h"
#include "GamePieceModel.h"
#include "PhysicsEngine.h"
#include "Types.h"


class WorldModel
{
friend class Scene;
public:
    /**
     * Constructor
     */
    WorldModel(ConfigReader& config, double timestamp);

    /**
     * Updates the field, vehicle, and game piece models
     */
    void update(double timestamp);

    /**
     * Resets the field
     */
    void reset();

    /**
     * Returns the world state of the sim
     */
    SimState getSimState();

    /**
     * Getter for vehicle model
     */
    VehicleModel& vehicleModel(unsigned int i) { return _vehicleModels.at(i); }

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
    std::vector<VehicleModel> _vehicleModels;
    PhysicsEngine _physicsEngine;
    std::vector<GamePieceModel> _gamePieceModels;
};
