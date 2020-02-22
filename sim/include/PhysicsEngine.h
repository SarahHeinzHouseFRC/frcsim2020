/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_PHYSICSENGINE_H
#define ROBOT_SIM_PHYSICSENGINE_H

#include "box2d/box2d.h"
#include "FieldModel.h"
#include "VehicleModel.h"
#include "GamePieceModel.h"


class PhysicsEngine
{
public:
    /**
     * Default constructor
     */
    PhysicsEngine() = default;

    /**
     * Constructor
     */
    PhysicsEngine(const FieldModel& fieldModel, const VehicleModel& vehicleModel, const std::vector<GamePieceModel>& gamePieceModels, double timestamp);

    /**
     * Detects collisions between the field and field actors
     */
    void update(FieldModel& fieldModel, VehicleModel& vehicleModel, std::vector<GamePieceModel>& gamePieceModels, double currTimestamp);

    /**
     * Deletes all box2d bodies and recreates from them from the models
     */
    void reset(const FieldModel& fieldModel, const VehicleModel& vehicleModel, const std::vector<GamePieceModel>& gamePieceModels);

private:
    class CollisionListener : public b2ContactListener
    {
    public:
        /**
         * Constructor
         */
        CollisionListener() = default;

        /**
         * Called when two fixtures begin to touch
         */
        virtual void BeginContact(b2Contact* contact)
        {
            // Call the FieldModel's collision callback iff the vehicle collided with the field
            auto bodyUserDataA = static_cast<BaseModel*>(contact->GetFixtureA()->GetBody()->GetUserData());
            BaseModel::ModelType typeA = static_cast<BaseModel*>(bodyUserDataA)->modelType();
            auto bodyUserDataB = static_cast<BaseModel*>(contact->GetFixtureB()->GetBody()->GetUserData());
            BaseModel::ModelType typeB = static_cast<BaseModel*>(bodyUserDataB)->modelType();
            if (typeA == BaseModel::VEHICLE_MODEL && typeB == BaseModel::FIELD_MODEL)
            {
                bodyUserDataB->isInCollision(true);
            }
            else if (typeA == BaseModel::FIELD_MODEL && typeB == BaseModel::VEHICLE_MODEL)
            {
                bodyUserDataA->isInCollision(true);
            }
        }

        /**
         * Called when two fixtures cease to touch
         */
        virtual void EndContact(b2Contact* contact)
        {
            // Call the FieldModel's collision callback iff the vehicle collided with the field
            auto bodyUserDataA = static_cast<BaseModel*>(contact->GetFixtureA()->GetBody()->GetUserData());
            BaseModel::ModelType typeA = static_cast<BaseModel*>(bodyUserDataA)->modelType();
            auto bodyUserDataB = static_cast<BaseModel*>(contact->GetFixtureB()->GetBody()->GetUserData());
            BaseModel::ModelType typeB = static_cast<BaseModel*>(bodyUserDataB)->modelType();
            if (typeA == BaseModel::VEHICLE_MODEL && typeB == BaseModel::FIELD_MODEL)
            {
                bodyUserDataB->isInCollision(false);
            }
            else if (typeA == BaseModel::FIELD_MODEL && typeB == BaseModel::VEHICLE_MODEL)
            {
                bodyUserDataA->isInCollision(false);
            }
        }
    };

    /**
     * Initializes field body from field model
     */
    void initFieldBodies(b2World* world, const FieldModel& fieldModel);

    /**
     * Initializes vehicle body from vehicle model
     */
    b2Body* initVehicleBody(b2World* world, const VehicleModel& vehicleModel);

    /**
     * Initializes all game pieces from their models
     */
    std::vector<b2Body*> initGamePieceBodies(b2World* world, const std::vector<GamePieceModel>& gamePieceModels);

    b2Vec2 _gravity;
    std::unique_ptr<b2World> _world;
    b2Body* _vehicleBody;
    b2PolygonShape _ingestibleRegionCenterShape;
    b2PolygonShape _ingestibleRegionLeftShape;
    b2PolygonShape _ingestibleRegionRightShape;
    b2PolygonShape _tubeRegion;
    b2CircleShape _gamePieceShape;
    std::vector<b2Body*> _gamePieceBodies;
    std::vector<b2Body*> _ingestibleCenterGamePieceBodies; // Game pieces in the center ingestible region
    std::vector<b2Body*> _ingestibleLeftGamePieceBodies; // Game pieces in the left ingestible region
    std::vector<b2Body*> _ingestibleRightGamePieceBodies; // Game pieces in the right ingestible region
    std::vector<b2Body*> _tubeGamePieceBodies; // Game pieces in the tube region
    int32 _velocityIterations;
    int32 _positionIterations;
    double _prevTimestamp;
    float _muGamePiece;
};


#endif //ROBOT_SIM_PHYSICSENGINE_H
