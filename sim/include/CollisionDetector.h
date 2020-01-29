/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_COLLISIONDETECTOR_H
#define ROBOT_SIM_COLLISIONDETECTOR_H

#include "box2d/box2d.h"
#include "Geometry.h"
#include "WorldModel.h"


class CollisionDetector
{
public:
    /**
     * Constructor
     */
    CollisionDetector(const WorldModel& wm, double timestamp);

    /**
     * Detects collisions between the field and field actors
     */
    void detectCollisions(WorldModel& wm, double currTimestamp);

private:
    b2Vec2 _gravity;
    b2World _world;
    b2Body* _vehicleBody;
    std::vector<b2Body*> _gamePieceBodies;
    int32 _velocityIterations;
    int32 _positionIterations;
    double _prevTimestamp;
    float _muGamePiece;
};


#endif //ROBOT_SIM_COLLISIONDETECTOR_H
