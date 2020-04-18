/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_PHYSICSENGINE_H
#define ROBOT_SIM_PHYSICSENGINE_H

#include "Box2D/Box2D.h"
#include "FieldModel.h"
#include "VehicleModel.h"
#include "GamePieceModel.h"
#include "PhysicsEngineLidar.h"


struct VehiclePhysicsModel
{
    b2Body* body;
    b2PolygonShape ingestibleRegionCenterShape;
    b2PolygonShape ingestibleRegionLeftShape;
    b2PolygonShape ingestibleRegionRightShape;
    b2PolygonShape tubeRegionShape;
    std::vector<b2Body*> ingestibleCenterGamePieceBodies; // Game pieces in the center ingestible region
    std::vector<b2Body*> ingestibleLeftGamePieceBodies; // Game pieces in the left ingestible region
    std::vector<b2Body*> ingestibleRightGamePieceBodies; // Game pieces in the right ingestible region
    std::vector<b2Body*> tubeGamePieceBodies; // Game pieces in the tube region
};



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
    PhysicsEngine(const ConfigReader& config, const FieldModel& fieldModel, const std::vector<VehicleModel>& vehicleModels, const std::vector<GamePieceModel>& gamePieceModels, double timestamp);

    // TODO: Add destructor that destroys all bodies

    /**
     * Detects collisions between the field and field actors
     */
    void update(FieldModel& fieldModel, std::vector<VehicleModel>& vehicleModels, std::vector<GamePieceModel>& gamePieceModels, double currTimestamp);

    /**
     * Deletes all box2d bodies and recreates from them from the models
     */
    void reset(const FieldModel& fieldModel, std::vector<VehicleModel>& vehicleModels, const std::vector<GamePieceModel>& gamePieceModels);

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
    VehiclePhysicsModel initVehiclePhysicsModel(b2World* world, const VehicleModel& vehicleModel);

    /**
     * Initializes all game pieces from their models
     */
    std::vector<b2Body*> initGamePieceBodies(b2World* world, const std::vector<GamePieceModel>& gamePieceModels);

    b2Vec2 _gravity;
    std::unique_ptr<b2World> _world;
    std::vector<VehiclePhysicsModel> _vehiclePhysicsModels;
    b2PolygonShape _blueGoalRegion;
    b2PolygonShape _redGoalRegion;
    b2CircleShape _gamePieceShape;
    std::vector<b2Body*> _gamePieceBodies;
    std::vector<b2Body*> _redGoalGamePieceBodies; // Game pieces in the red goal region
    std::vector<b2Body*> _blueGoalGamePieceBodies; // Game pieces in the blue goal region
    int32 _velocityIterations;
    int32 _positionIterations;
    double _prevTimestamp;
    float _muGamePiece;
    int _blueOuttaken; // Count of how many balls have been outtaken from blue goal (so we can calculate score)
    int _redOuttaken; // Count of how many balls have been outtaken from red goal (so we can calculate score)
    std::unique_ptr<PhysicsEngineLidar> _lidar;
};


#endif //ROBOT_SIM_PHYSICSENGINE_H
