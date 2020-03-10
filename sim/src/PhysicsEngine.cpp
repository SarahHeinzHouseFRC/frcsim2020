/**
 * Copyright (c) 2020 Team 3260
 */

#include <GamePieceModel.h>
#include "Geometry.h"
#include "PhysicsEngine.h"

using namespace Geometry;

/** Coefficient of friction of the game pieces */
#define MU_GAME_PIECE 0.1


enum CollisionCategory
{
    CATEGORY_A = 0x0001, // INGESTIBLE_REGION
    CATEGORY_B = 0x0002, // TUBE_REGION
    CATEGORY_C = 0x0004, // GOAL_REGION
    CATEGORY_O = 0x0008, // BOUNDARY
    CATEGORY_OMEGA = 0x0010, // UNINGESTED_GAME_PIECE
    CATEGORY_ALPHA = 0x0020, // INGESTIBLE_GAME_PIECE
    CATEGORY_BETA = 0x0040, // INGESTED_GAME_PIECE
};



enum CollisionMask
{
    MASK_A = CATEGORY_C | CATEGORY_O, // INGESTIBLE_REGION
    MASK_B = CATEGORY_OMEGA | CATEGORY_O | CATEGORY_C, // TUBE_REGION
    MASK_C = CATEGORY_A | CATEGORY_B | CATEGORY_C | CATEGORY_O | CATEGORY_OMEGA | CATEGORY_ALPHA, // GOAL_REGION
    MASK_O = CATEGORY_A | CATEGORY_B | CATEGORY_C | CATEGORY_O | CATEGORY_OMEGA | CATEGORY_ALPHA | CATEGORY_BETA, // BOUNDARY
    MASK_OMEGA = CATEGORY_B | CATEGORY_C | CATEGORY_O | CATEGORY_OMEGA | CATEGORY_ALPHA | CATEGORY_BETA, // UNINGESTED_GAME_PIECE
    MASK_ALPHA = CATEGORY_C | CATEGORY_O | CATEGORY_OMEGA | CATEGORY_ALPHA | CATEGORY_BETA, // INGESTIBLE_GAME_PIECE
    MASK_BETA = CATEGORY_O | CATEGORY_ALPHA | CATEGORY_BETA, // INGESTED_GAME_PIECE
};



PhysicsEngine::PhysicsEngine(const FieldModel& fieldModel,
                             const std::vector<VehicleModel>& vehicleModels,
                             const std::vector<GamePieceModel>& gamePieceModels,
                             double timestamp) :
        _prevTimestamp(timestamp),
        _gravity(0.0f, 0.0f),
        _muGamePiece(MU_GAME_PIECE),
        _blueOuttaken(0),
        _redOuttaken(0)
{
    _world = std::make_unique<b2World>(_gravity);

    // Add collision listener
    _world->SetContactListener(new CollisionListener);

    // Field static bodies
    initFieldBodies(_world.get(), fieldModel);

    // Vehicle dynamic body
    for (const auto& vehicleModel : vehicleModels)
    {
        _vehiclePhysicsModels.push_back(initVehiclePhysicsModel(_world.get(), vehicleModel));
    }

    // Game piece dynamic bodies
    _gamePieceBodies = initGamePieceBodies(_world.get(), gamePieceModels);

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    _velocityIterations = 6;
    _positionIterations = 2;
}



void PhysicsEngine::update(FieldModel& fieldModel, std::vector<VehicleModel>& vehicleModels, std::vector<GamePieceModel>& gamePieceModels, double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;
    if (elapsedTime == 0) { return; }

    // Update the vehicle velocities
    for (int i=0; i<_vehiclePhysicsModels.size(); i++)
    {
        _vehiclePhysicsModels.at(i).body->SetLinearVelocity(b2Vec2(vehicleModels.at(i)._state.pose.vx, vehicleModels.at(i)._state.pose.vy));
        _vehiclePhysicsModels.at(i).body->SetAngularVelocity(vehicleModels.at(i)._state.pose.omega);
    }

    // Outtake balls if the vehicles call for it
    for (auto& vehicleModel : vehicleModels)
    {
        // TODO: This framework is really starting to break down, should the physics engine really be handling this?
        if (vehicleModel._outtake)
        {
            if (vehicleModel._alliance == "Blue" && !_blueGoalGamePieceBodies.empty())
            {
                auto* gamePieceBody = _blueGoalGamePieceBodies.front();
                gamePieceBody->SetTransform(b2Vec2(fieldModel._blueOuttake.x, fieldModel._blueOuttake.y), 0);
                gamePieceBody->SetLinearVelocity({ 0, -1 });
                _blueOuttaken++;
            }
            if (vehicleModel._alliance == "Red" && !_redGoalGamePieceBodies.empty())
            {
                auto* gamePieceBody = _redGoalGamePieceBodies.front();
                gamePieceBody->SetTransform(b2Vec2(fieldModel._redOuttake.x, fieldModel._redOuttake.y), 0);
                gamePieceBody->SetLinearVelocity({ 0, 1 });
                _redOuttaken++;
            }
            vehicleModel._outtake = false;
        }
    }

    // Manually apply friction to the game pieces
    for (const auto& gamePieceBody : _gamePieceBodies)
    {
        b2Vec2 velocity = gamePieceBody->GetLinearVelocity();
        if (velocity.LengthSquared() > 0)
        {
            b2Vec2 frictionForce = -velocity;
            frictionForce.Normalize();
            frictionForce *= _muGamePiece;
            gamePieceBody->ApplyForceToCenter(frictionForce, true);
        }
    }

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world->Step(elapsedTime, _velocityIterations, _positionIterations);

    // Manually move ingestible/tube game pieces
    for (int i=0; i<vehicleModels.size(); i++)
    {
        auto& vehicleModel = vehicleModels.at(i);
        auto& vehiclePhysicsModel = _vehiclePhysicsModels.at(i);

        for (b2Body* gamePieceBody : vehiclePhysicsModel.ingestibleCenterGamePieceBodies)
        {
            // Enter a slave state, move according to center intake motor and half the vehicle speed
            float velocityMagnitude = vehicleModel._state.intakeCenterMotorSpeed;
            b2Vec2 velocity = vehiclePhysicsModel.body->GetWorldVector(b2Vec2(-1, 0));
            velocity *= velocityMagnitude;
            b2Vec2 vehicleVelocity = vehiclePhysicsModel.body->GetLinearVelocityFromWorldPoint(gamePieceBody->GetPosition());
            vehicleVelocity *= 0.5;
            velocity += vehicleVelocity;
            gamePieceBody->SetLinearVelocity(velocity);
        }
        for (b2Body* gamePieceBody : vehiclePhysicsModel.ingestibleLeftGamePieceBodies)
        {
            // Enter a slave state, move according to left intake motor and half the vehicle speed
            float velocityMagnitude = vehicleModel._state.intakeLeftMotorSpeed;
            b2Vec2 velocity = vehiclePhysicsModel.body->GetWorldVector(b2Vec2(0, -1));
            velocity *= velocityMagnitude;
            b2Vec2 vehicleVelocity = vehiclePhysicsModel.body->GetLinearVelocityFromWorldPoint(gamePieceBody->GetPosition());
            vehicleVelocity *= 0.5;
            velocity += vehicleVelocity;
            gamePieceBody->SetLinearVelocity(velocity);
        }
        for (b2Body* gamePieceBody : vehiclePhysicsModel.ingestibleRightGamePieceBodies)
        {
            // Enter a slave state, move according to right intake motor and half the vehicle speed
            float velocityMagnitude = vehicleModel._state.intakeRightMotorSpeed;
            b2Vec2 velocity = vehiclePhysicsModel.body->GetWorldVector(b2Vec2(0, 1));
            velocity *= velocityMagnitude;
            b2Vec2 vehicleVelocity = vehiclePhysicsModel.body->GetLinearVelocityFromWorldPoint(gamePieceBody->GetPosition());
            vehicleVelocity *= 0.5;
            velocity += vehicleVelocity;
            gamePieceBody->SetLinearVelocity(velocity);
        }
        for (b2Body* gamePieceBody : vehiclePhysicsModel.tubeGamePieceBodies)
        {
            // Enter a slave state, move according to tube motor and the vehicle speed
            float velocityMagnitude = vehicleModel._state.tubeMotorSpeed;
            b2Vec2 velocity = vehiclePhysicsModel.body->GetWorldVector(b2Vec2(-1, 0));
            velocity *= velocityMagnitude;
            b2Vec2 vehicleVelocity = vehiclePhysicsModel.body->GetLinearVelocityFromWorldPoint(gamePieceBody->GetPosition());
            velocity += vehicleVelocity;
            gamePieceBody->SetLinearVelocity(velocity);
        }

        // Update the vehicle model
        {
            b2Vec2 position = vehiclePhysicsModel.body->GetPosition();
            float angle = vehiclePhysicsModel.body->GetAngle();
            b2Vec2 velocity = vehiclePhysicsModel.body->GetLinearVelocity();

            vehicleModel._state.pose.x = position.x;
            vehicleModel._state.pose.y = position.y;
            vehicleModel._state.pose.theta = angle;
        }
    }

    // Update the game piece models
    {
        for (int i=0; i<_gamePieceBodies.size(); i++)
        {
            b2Vec2 position = _gamePieceBodies[i]->GetPosition();
            b2Vec2 velocity = _gamePieceBodies[i]->GetLinearVelocity();
            float angle = _gamePieceBodies[i]->GetAngle();

            gamePieceModels[i]._state.pose.x = position.x;
            gamePieceModels[i]._state.pose.y = position.y;
        }
    }

    // Determine the balls in each region
    for (auto& vehiclePhysicsModel : _vehiclePhysicsModels)
    {
        vehiclePhysicsModel.ingestibleCenterGamePieceBodies.clear();
        vehiclePhysicsModel.ingestibleLeftGamePieceBodies.clear();
        vehiclePhysicsModel.ingestibleRightGamePieceBodies.clear();
        vehiclePhysicsModel.tubeGamePieceBodies.clear();
    }

    _blueGoalGamePieceBodies.clear();
    _redGoalGamePieceBodies.clear();

    for (const auto& gamePieceBody : _gamePieceBodies)
    {
        b2Vec2 gamePiecePosition = gamePieceBody->GetWorldCenter();
        auto gamePieceModel = (GamePieceModel*) gamePieceBody->GetUserData();

        bool inARegion = false;
        for (int i=0; i<vehicleModels.size(); i++)
        {
            auto& vehicleModel = vehicleModels.at(i);
            auto& vehiclePhysicsModel = _vehiclePhysicsModels.at(i);

            b2Transform vehicleTf = vehiclePhysicsModel.body->GetTransform();
            bool overlapCenter = vehiclePhysicsModel.ingestibleRegionCenterShape.TestPoint(vehicleTf, gamePiecePosition);
            bool overlapLeft = vehiclePhysicsModel.ingestibleRegionLeftShape.TestPoint(vehicleTf, gamePiecePosition);
            bool overlapRight = vehiclePhysicsModel.ingestibleRegionRightShape.TestPoint(vehicleTf, gamePiecePosition);
            bool overlapTube = vehiclePhysicsModel.tubeRegionShape.TestPoint(vehicleTf, gamePiecePosition);

            if (overlapCenter)
            {
                b2Filter filter = gamePieceBody->GetFixtureList()[0].GetFilterData();
                filter.categoryBits = CATEGORY_ALPHA;
                filter.maskBits = MASK_ALPHA;
                gamePieceBody->GetFixtureList()[0].SetFilterData(filter);

                vehiclePhysicsModel.ingestibleCenterGamePieceBodies.push_back(gamePieceBody);
                gamePieceModel->_state.ingestion = GamePieceModel::CENTER_INTAKE;
                inARegion = true;
            }
            else if (overlapLeft)
            {
                b2Filter filter = gamePieceBody->GetFixtureList()[0].GetFilterData();
                filter.categoryBits = CATEGORY_ALPHA;
                filter.maskBits = MASK_ALPHA;
                gamePieceBody->GetFixtureList()[0].SetFilterData(filter);

                vehiclePhysicsModel.ingestibleLeftGamePieceBodies.push_back(gamePieceBody);
                gamePieceModel->_state.ingestion = GamePieceModel::LEFT_INTAKE;
                inARegion = true;
            }
            else if (overlapRight)
            {
                b2Filter filter = gamePieceBody->GetFixtureList()[0].GetFilterData();
                filter.categoryBits = CATEGORY_ALPHA;
                filter.maskBits = MASK_ALPHA;
                gamePieceBody->GetFixtureList()[0].SetFilterData(filter);

                vehiclePhysicsModel.ingestibleRightGamePieceBodies.push_back(gamePieceBody);
                gamePieceModel->_state.ingestion = GamePieceModel::RIGHT_INTAKE;
                inARegion = true;
            }
            else if (overlapTube)
            {
                b2Filter filter = gamePieceBody->GetFixtureList()[0].GetFilterData();
                filter.categoryBits = CATEGORY_BETA;
                filter.maskBits = MASK_BETA;
                gamePieceBody->GetFixtureList()[0].SetFilterData(filter);

                vehiclePhysicsModel.tubeGamePieceBodies.push_back(gamePieceBody);
                b2Vec2 rearLeftCornerLocal(0.05, 0.11);
                b2Vec2 rearRightCornerLocal(0.05, -0.11);
                b2Vec2 rearEdge = rearLeftCornerLocal - rearRightCornerLocal;
                b2Vec2 rearEdgePerp(rearEdge.y, rearEdge.x);
                rearEdgePerp.Normalize();
                b2Vec2 gamePiecePosLocal = vehiclePhysicsModel.body->GetLocalPoint(gamePieceBody->GetPosition());
                b2Vec2 vecToGamePiece = gamePiecePosLocal - rearRightCornerLocal;
                float dist = -b2Dot(rearEdgePerp, vecToGamePiece);
                gamePieceModel->_state.pose.z = dist * (0.45 / 0.51);
                gamePieceModel->_state.ingestion = GamePieceModel::TUBE;
                inARegion = true;
            }
        }

        b2Transform fieldTf({ 0, 0 }, b2Rot(0));
        bool inBlueGoalRegion = _blueGoalRegion.TestPoint(fieldTf, gamePiecePosition);
        bool inRedGoalRegion = _redGoalRegion.TestPoint(fieldTf, gamePiecePosition);

        if (inBlueGoalRegion)
        {
            gamePieceModel->_state.pose.vz = 0;
            gamePieceModel->_state.pose.z = 0.38 + gamePieceModel->_radius;
            gamePieceModel->_state.ingestion = GamePieceModel::BLUE_LOW_GOAL;
            _blueGoalGamePieceBodies.push_back(gamePieceBody);
        }
        else if (inRedGoalRegion)
        {
            gamePieceModel->_state.pose.vz = 0;
            gamePieceModel->_state.pose.z = 0.38 + gamePieceModel->_radius;
            gamePieceModel->_state.ingestion = GamePieceModel::RED_LOW_GOAL;
            _redGoalGamePieceBodies.push_back(gamePieceBody);
        }
        else if (!inARegion)
        {
            b2Filter filter = gamePieceBody->GetFixtureList()[0].GetFilterData();

            if (gamePieceModel->_state.pose.z > 0)
            {
                // Allow balls to go into the goal as long as they're off the ground
                filter.categoryBits = CATEGORY_BETA;
                filter.maskBits = MASK_BETA;
            }
            else
            {
                // All other balls on the ground are not allowed into the goal
                filter.categoryBits = CATEGORY_OMEGA;
                filter.maskBits = MASK_OMEGA;
            }

            gamePieceBody->GetFixtureList()[0].SetFilterData(filter);

            if (gamePieceModel->_state.pose.z > 0)
            {
                // When balls are unsupported, they should fall to the floor
                const double az = -9.81;
                gamePieceModel->_state.pose.vz += elapsedTime * az;
                gamePieceModel->_state.pose.z += gamePieceModel->_state.pose.vz;
            }
            else
            {
                // Balls on the floor should stay on the floor
                gamePieceModel->_state.pose.vz = 0;
                gamePieceModel->_state.pose.z = 0;
            }

            gamePieceModel->_state.ingestion = GamePieceModel::NOT_INGESTED;
        }
    }

    fieldModel._blueScore = _blueGoalGamePieceBodies.size() + _blueOuttaken;
    fieldModel._redScore = _redGoalGamePieceBodies.size() + _redOuttaken;

    _prevTimestamp = currTimestamp;
}



void PhysicsEngine::reset(const FieldModel& fieldModel, std::vector<VehicleModel>& vehicleModels, const std::vector<GamePieceModel>& gamePieceModels)
{
    // Reset vehicle
    for (int i=0; i<vehicleModels.size(); i++)
    {
        _vehiclePhysicsModels.at(i).body->SetTransform(b2Vec2(vehicleModels.at(i)._state.pose.x, vehicleModels.at(i)._state.pose.y), vehicleModels.at(i)._state.pose.theta);
    }

    // Reset game pieces
    for (int i=0; i<gamePieceModels.size(); i++)
    {
        const GamePieceModel& model = gamePieceModels.at(i);
        b2Body* body = _gamePieceBodies.at(i);
        body->SetTransform(b2Vec2(model._initialX, model._initialY), 0);
        body->SetLinearVelocity({ 0, 0 });
    }
}



void PhysicsEngine::initFieldBodies(b2World* world, const FieldModel& fieldModel)
{
    // Field exterior static body
    Polygon2d exteriorPolygon = fieldModel._exteriorPolygon;
    for (const auto& edge : exteriorPolygon.edges())
    {
        b2BodyDef wallDef;
        b2Body* wallBody = world->CreateBody(&wallDef);
        b2Vec2 v1(edge.a.x, edge.a.y);
        b2Vec2 v2(edge.b.x, edge.b.y);
        b2EdgeShape wallShape;
        wallShape.Set(v1, v2);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &wallShape;
        fixtureDef.density = 0;
        fixtureDef.filter.categoryBits = CATEGORY_O;
        fixtureDef.filter.maskBits = MASK_O;
        wallBody->CreateFixture(&fixtureDef);
        wallBody->SetUserData((void*) &fieldModel);
    }

    // Field interior static bodies
    for (const auto& interiorPolygon : fieldModel._interiorPolygons)
    {
        for (const auto& edge : interiorPolygon.edges())
        {
            b2BodyDef wallDef;
            b2Body* wallBody = world->CreateBody(&wallDef);
            b2Vec2 v1(edge.a.x, edge.a.y);
            b2Vec2 v2(edge.b.x, edge.b.y);
            b2EdgeShape wallShape;
            wallShape.Set(v1, v2);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &wallShape;
            fixtureDef.density = 0;
            fixtureDef.filter.categoryBits = CATEGORY_O;
            fixtureDef.filter.maskBits = MASK_O;
            wallBody->CreateFixture(&fixtureDef);
            wallBody->SetUserData((void*) &fieldModel);
        }
    }

    // Goal regions
    {
        b2BodyDef goalBodyDef;
        b2Body* goalBody = world->CreateBody(&goalBodyDef);
        int numVertices = fieldModel._blueGoal.numVertices();
        b2Vec2 vertices[numVertices];
        for (int i=0; i<numVertices; i++)
        {
            Vertex2d v = fieldModel._blueGoal.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        _blueGoalRegion.Set(vertices, numVertices);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &_blueGoalRegion;
        fixtureDef.density = 0;
        fixtureDef.friction = 0.3f;
        fixtureDef.filter.categoryBits = CATEGORY_C;
        fixtureDef.filter.maskBits = MASK_C;

        // Add the shape to the body
        goalBody->CreateFixture(&fixtureDef);
        goalBody->SetUserData((void*) &fieldModel);
    }

    {
        b2BodyDef goalBodyDef;
        b2Body* goalBody = world->CreateBody(&goalBodyDef);
        int numVertices = fieldModel._redGoal.numVertices();
        b2Vec2 vertices[numVertices];
        for (int i=0; i<numVertices; i++)
        {
            Vertex2d v = fieldModel._redGoal.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        _redGoalRegion.Set(vertices, numVertices);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &_redGoalRegion;
        fixtureDef.density = 0;
        fixtureDef.friction = 0.3f;
        fixtureDef.filter.categoryBits = CATEGORY_C;
        fixtureDef.filter.maskBits = MASK_C;

        // Add the shape to the body
        goalBody->CreateFixture(&fixtureDef);
        goalBody->SetUserData((void*) &fieldModel);
    }
}



VehiclePhysicsModel PhysicsEngine::initVehiclePhysicsModel(b2World* world, const VehicleModel& vehicleModel)
{
    VehiclePhysicsModel vehiclePhysicsModel;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef vehicleBodyDef;
    vehicleBodyDef.type = b2_dynamicBody;
    vehicleBodyDef.position.Set(vehicleModel._state.pose.x, vehicleModel._state.pose.y);
    vehicleBodyDef.angle = vehicleModel._state.pose.theta;
    vehiclePhysicsModel.body = world->CreateBody(&vehicleBodyDef);

    // Define a front left polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonFrontLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define a rear left polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonRearLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define a front right polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonFrontRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define a rear right polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonRearRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define front left bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperFrontLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define front right bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperFrontRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define left bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define right bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define rear left bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperRearLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define rear left bumper fixture and shape
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonBumperRearRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_O;
        vehicleFixtureDef.filter.maskBits = MASK_O;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define center ingestible region
    {
        b2FixtureDef vehicleFixtureDef;
        Polygon2d bounds = vehicleModel._ingestibleRegionCenter;
        b2Vec2 verticesCenter[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            verticesCenter[i] = b2Vec2(v.x, v.y);
        }
        vehiclePhysicsModel.ingestibleRegionCenterShape.Set(verticesCenter, bounds.numVertices());
        vehicleFixtureDef.shape = &vehiclePhysicsModel.ingestibleRegionCenterShape;
        vehicleFixtureDef.density = 0;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_A;
        vehicleFixtureDef.filter.maskBits = MASK_A;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define left ingestible region
    {
        b2FixtureDef vehicleFixtureDef;
        Polygon2d bounds = vehicleModel._ingestibleRegionLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehiclePhysicsModel.ingestibleRegionLeftShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehiclePhysicsModel.ingestibleRegionLeftShape;
        vehicleFixtureDef.density = 0;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_A;
        vehicleFixtureDef.filter.maskBits = MASK_A;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define right ingestible region
    {
        b2FixtureDef vehicleFixtureDef;
        Polygon2d bounds = vehicleModel._ingestibleRegionRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehiclePhysicsModel.ingestibleRegionRightShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehiclePhysicsModel.ingestibleRegionRightShape;
        vehicleFixtureDef.density = 0;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_A;
        vehicleFixtureDef.filter.maskBits = MASK_A;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Define ingested region
    {
        b2FixtureDef vehicleFixtureDef;
        Polygon2d bounds = vehicleModel._tubeRegion;
        b2Vec2 vertices[bounds.numVertices()];
        for (int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehiclePhysicsModel.tubeRegionShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehiclePhysicsModel.tubeRegionShape;
        vehicleFixtureDef.density = 0;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = CATEGORY_B;
        vehicleFixtureDef.filter.maskBits = MASK_B;

        // Add the shape to the body
        vehiclePhysicsModel.body->CreateFixture(&vehicleFixtureDef);
    }

    // Add user data
    vehiclePhysicsModel.body->SetUserData((void*) &vehicleModel);

    return vehiclePhysicsModel;
}



std::vector<b2Body*> PhysicsEngine::initGamePieceBodies(b2World* world, const std::vector<GamePieceModel>& gamePieceModels)
{
    std::vector<b2Body*> gamePieceBodies;
    for (const auto& gamePieceModel : gamePieceModels)
    {
        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef gamePieceBodyDef;
        gamePieceBodyDef.type = b2_dynamicBody;
        gamePieceBodyDef.position.Set(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y);
        b2Body* gamePieceBody = world->CreateBody(&gamePieceBodyDef);

        // Define another box shape for our dynamic body.
        _gamePieceShape.m_radius = gamePieceModel._radius;

        b2FixtureDef gamePieceFixtureDef;
        gamePieceFixtureDef.shape = &_gamePieceShape;
        gamePieceFixtureDef.restitution = 0.5f;

        gamePieceFixtureDef.density = 1.0f;
        gamePieceFixtureDef.friction = 0.3f;
        gamePieceFixtureDef.filter.categoryBits = CATEGORY_OMEGA;
        gamePieceFixtureDef.filter.maskBits = MASK_OMEGA;
        gamePieceBody->CreateFixture(&gamePieceFixtureDef);
        gamePieceBody->SetUserData((void*) &gamePieceModel);
        gamePieceBodies.push_back(gamePieceBody);
    }

    return gamePieceBodies;
}
