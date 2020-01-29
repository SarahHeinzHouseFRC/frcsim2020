/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_COLLISIONDETECTOR_H
#define ROBOT_SIM_COLLISIONDETECTOR_H

#include "box2d/box2d.h"
#include "Geometry.h"
#include "WorldModel.h"
#include "BaseModel.h"


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
    class CollisionListener : public b2ContactListener
    {
    public:
        /**
         * Constructor
         */
        CollisionListener() {};

        /**
         * Called when two fixtures begin to touch
         */
        virtual void BeginContact(b2Contact* contact)
        {
            BaseModel::ModelType typeA = BaseModel::NONE_MODEL, typeB = BaseModel::NONE_MODEL;
            void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
            if (bodyUserDataA)
            {
                typeA = static_cast<BaseModel*>(bodyUserDataA)->modelType();
            }
            void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
            if (bodyUserDataB)
            {
                typeB = static_cast<BaseModel*>(bodyUserDataB)->modelType();
            }
            if (typeA == BaseModel::VEHICLE_MODEL && typeB == BaseModel::FIELD_MODEL)
            {
                static_cast<BaseModel*>(bodyUserDataB)->hasCollision(true);
            }
            else if (typeA == BaseModel::FIELD_MODEL && typeB == BaseModel::VEHICLE_MODEL)
            {
                static_cast<BaseModel*>(bodyUserDataA)->hasCollision(true);
            }
        }

        /**
         * Called when two fixtures cease to touch
         */
        virtual void EndContact(b2Contact* contact)
        {
            void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
            static_cast<BaseModel*>(bodyUserDataA)->hasCollision(false);
            void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
            static_cast<BaseModel*>(bodyUserDataB)->hasCollision(false);
        }
    };

    CollisionListener _collisionListener;
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
