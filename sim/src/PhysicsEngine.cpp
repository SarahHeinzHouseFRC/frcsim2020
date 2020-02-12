/**
 * Copyright (c) 2020 Team 3260
 */

#include <GamePieceModel.h>
#include "Geometry.h"
#include "PhysicsEngine.h"

using namespace Geometry;

/** Coefficient of friction of the game pieces */
#define MU_GAME_PIECE 0.1


PhysicsEngine::PhysicsEngine(const FieldModel& fieldModel,
                             const VehicleModel& vehicleModel,
                             const std::vector<GamePieceModel>& gamePieceModels,
                             double timestamp) :
        _prevTimestamp(timestamp),
        _gravity(0.0f, 0.0f),
        _muGamePiece(MU_GAME_PIECE)
{
    _world = std::make_unique<b2World>(_gravity);

    // Add collision listener
    _world->SetContactListener(new CollisionListener);

    // Field static bodies
    initFieldBodies(_world.get(), fieldModel);

    // Vehicle dynamic body
    _vehicleBody = initVehicleBody(_world.get(), vehicleModel);

    // Game piece dynamic bodies
    _gamePieceBodies = initGamePieceBodies(_world.get(), gamePieceModels);

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    _velocityIterations = 6;
    _positionIterations = 2;
}



void PhysicsEngine::update(FieldModel& fieldModel, VehicleModel& vehicleModel, std::vector<GamePieceModel>& gamePieceModels, double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;
    if (elapsedTime == 0) { return; }

    // Update the vehicle velocities
    _vehicleBody->SetLinearVelocity(b2Vec2(vehicleModel._state.pose.vx, vehicleModel._state.pose.vy));
    _vehicleBody->SetAngularVelocity(vehicleModel._state.pose.omega);

    // Manually apply friction to the game pieces
    {
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
    }

    // Manually move ingested game pieces
    for (GamePieceModel* gamePieceModel : vehicleModel._ingested)
    {
        // Enter a slave state
    }

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world->Step(elapsedTime, _velocityIterations, _positionIterations);

    // Update the vehicle model
    {
        b2Vec2 position = _vehicleBody->GetPosition();
        float angle = _vehicleBody->GetAngle();
        b2Vec2 velocity = _vehicleBody->GetLinearVelocity();

        vehicleModel._state.pose.x = position.x;
        vehicleModel._state.pose.y = position.y;
        vehicleModel._state.pose.theta = angle;
    }

    // Update the game piece models
    {
        for (unsigned int i=0; i<_gamePieceBodies.size(); i++)
        {
            b2Vec2 position = _gamePieceBodies[i]->GetPosition();
            b2Vec2 velocity = _gamePieceBodies[i]->GetLinearVelocity();
            float angle = _gamePieceBodies[i]->GetAngle();

            gamePieceModels[i]._state.pose.x = position.x;
            gamePieceModels[i]._state.pose.y = position.y;
        }
    }

    // Get a list of balls in the ingestible region
    _ingestedGamePieceBodies.clear();
    vehicleModel._ingested.clear();
    for (const auto& gamePieceBody : _gamePieceBodies)
    {
        b2Transform xfA = _vehicleBody->GetTransform(), xfB = gamePieceBody->GetTransform();
        bool overlap = b2TestOverlap(&_ingestibleRegionShape, 0, &_gamePieceShape, 0, xfA, xfB);
        auto model = (GamePieceModel*) gamePieceBody->GetUserData();
        if (overlap)
        {
            _ingestedGamePieceBodies.push_back(gamePieceBody);
            vehicleModel._ingested.push_back(model);
            model->_state.ingestion = GamePieceModel::INGESTIBLE;
        }
        else
        {
            model->_state.ingestion = GamePieceModel::NOT_INGESTED;
        }
    }


    _prevTimestamp = currTimestamp;
}



void PhysicsEngine::reset(const FieldModel& fieldModel, const VehicleModel& vehicleModel, const std::vector<GamePieceModel>& gamePieceModels)
{
    // Reset vehicle
    _vehicleBody->SetTransform(b2Vec2(vehicleModel._state.pose.x, vehicleModel._state.pose.y), vehicleModel._state.pose.theta);

    // Reset game pieces
    for (unsigned int i=0; i<gamePieceModels.size(); i++)
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
    Polygon2d exteriorPolygon = fieldModel.exteriorPolygon();
    for (const auto& edge : exteriorPolygon.edges())
    {
        b2BodyDef wallDef;
        b2Body* wallBody = world->CreateBody(&wallDef);
        b2Vec2 v1(edge.a.x, edge.a.y);
        b2Vec2 v2(edge.b.x, edge.b.y);
        b2EdgeShape wallShape;
        wallShape.Set(v1, v2);
        wallBody->CreateFixture(&wallShape, 0);
        wallBody->SetUserData((void*) &fieldModel);
    }

    // Field interior static bodies
    for (const auto& interiorPolygon : fieldModel.interiorPolygons())
    {
        for (const auto& edge : interiorPolygon.edges())
        {
            b2BodyDef wallDef;
            b2Body* wallBody = world->CreateBody(&wallDef);
            b2Vec2 v1(edge.a.x, edge.a.y);
            b2Vec2 v2(edge.b.x, edge.b.y);
            b2EdgeShape wallShape;
            wallShape.Set(v1, v2);
            wallBody->CreateFixture(&wallShape, 0);
            wallBody->SetUserData((void*) &fieldModel);
        }
    }
}



b2Body* PhysicsEngine::initVehicleBody(b2World* world, const VehicleModel& vehicleModel)
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef vehicleBodyDef;
    vehicleBodyDef.type = b2_dynamicBody;
    vehicleBodyDef.position.Set(vehicleModel._state.pose.x, vehicleModel._state.pose.y);
    vehicleBodyDef.angle = vehicleModel._state.pose.theta;
    b2Body* vehicleBody = world->CreateBody(&vehicleBodyDef);

    // Define a left polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonLeft;
        b2Vec2 vertices[bounds.numVertices()];
        for (unsigned int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;

        // Add the shape to the body
        vehicleBody->CreateFixture(&vehicleFixtureDef);
    }

    // Define a left polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonRight;
        b2Vec2 vertices[bounds.numVertices()];
        for (unsigned int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;

        // Add the shape to the body
        vehicleBody->CreateFixture(&vehicleFixtureDef);
    }

    // Define a rear polygon shape for our dynamic body
    {
        b2FixtureDef vehicleFixtureDef;
        b2PolygonShape vehicleShape;
        Polygon2d bounds = vehicleModel._boundingPolygonRear;
        b2Vec2 vertices[bounds.numVertices()];
        for (unsigned int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        vehicleShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &vehicleShape;
        vehicleFixtureDef.density = (vehicleModel._mass) / 0.047f;
        vehicleFixtureDef.friction = 0.3f;

        // Add the shape to the body
        vehicleBody->CreateFixture(&vehicleFixtureDef);
    }

    // Define ingestible region
    {
        b2FixtureDef vehicleFixtureDef;
        Polygon2d bounds = vehicleModel._ingestibleRegion;
        b2Vec2 vertices[bounds.numVertices()];
        for (unsigned int i=0; i<bounds.numVertices(); i++)
        {
            Vertex2d v = bounds.vertices().at(i);
            vertices[i] = b2Vec2(v.x, v.y);
        }
        _ingestibleRegionShape.Set(vertices, bounds.numVertices());
        vehicleFixtureDef.shape = &_ingestibleRegionShape;
        vehicleFixtureDef.density = 0;
        vehicleFixtureDef.friction = 0.3f;
        vehicleFixtureDef.filter.categoryBits = 0x0002;
        vehicleFixtureDef.filter.maskBits = 0x0002;

        // Add the shape to the body
        vehicleBody->CreateFixture(&vehicleFixtureDef);
    }

    // Add user data
    vehicleBody->SetUserData((void*) &vehicleModel);

    return vehicleBody;
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
        gamePieceBody->CreateFixture(&gamePieceFixtureDef);
        gamePieceBody->SetUserData((void*) &gamePieceModel);
        gamePieceBodies.push_back(gamePieceBody);
    }

    return gamePieceBodies;
}
