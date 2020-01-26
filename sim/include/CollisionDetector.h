/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_COLLISIONDETECTOR_H
#define ROBOT_SIM_COLLISIONDETECTOR_H

#include "VehicleModel.h"
#include "FieldModel.h"
#include "GamePieceModel.h"
#include "Geometry.h"


class CollisionDetector
{
public:
    /**
     * Constructor
     */
    CollisionDetector();

    /**
     * Detects collisions between the field and field actors
     */
    void detectCollisions(FieldModel& fieldModel, VehicleModel& vehicleModel, GamePieceModel& gamePieceModel);
};


#endif //ROBOT_SIM_COLLISIONDETECTOR_H
