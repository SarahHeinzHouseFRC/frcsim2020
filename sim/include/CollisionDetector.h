/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_COLLISIONDETECTOR_H
#define ROBOT_SIM_COLLISIONDETECTOR_H

#include "box2d/box2d.h"
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

private:
    b2Vec2 _gravity;
    b2World _world;
    b2Body* _body;
    float _timeStep;
    int32 _velocityIterations;
    int32 _positionIterations;
};


#endif //ROBOT_SIM_COLLISIONDETECTOR_H
