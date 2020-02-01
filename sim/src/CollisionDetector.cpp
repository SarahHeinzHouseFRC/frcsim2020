/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <VehicleModel.h>
#include "CollisionDetector.h"

using namespace Geometry;


CollisionDetector::CollisionDetector(const WorldModel& wm, double timestamp) :
        _prevTimestamp(timestamp),
        _muGamePiece(0.1),
        _gravity(0.0f, 0.0f),
        _world(_gravity),
        _collisionListener()
{
    // Add collision listener
    _world.SetContactListener(&_collisionListener);

    // Field exterior static body
    Polygon2d exteriorPolygon = wm._fieldModel.exteriorPolygon();
    for (const auto& edge : exteriorPolygon.edges())
    {
        b2BodyDef wallDef;
        b2Body* wallBody = _world.CreateBody(&wallDef);
        b2Vec2 v1(edge.a.x, edge.a.y);
        b2Vec2 v2(edge.b.x, edge.b.y);
        b2EdgeShape wallShape;
        wallShape.Set(v1, v2);
        wallBody->CreateFixture(&wallShape, 0);
        wallBody->SetUserData((void*) &wm._fieldModel);
    }

    // Field interior static bodies
    for (const auto& interiorPolygon : wm._fieldModel.interiorPolygons())
    {
        for (const auto& edge : interiorPolygon.edges())
        {
            b2BodyDef wallDef;
            b2Body* wallBody = _world.CreateBody(&wallDef);
            b2Vec2 v1(edge.a.x, edge.a.y);
            b2Vec2 v2(edge.b.x, edge.b.y);
            b2EdgeShape wallShape;
            wallShape.Set(v1, v2);
            wallBody->CreateFixture(&wallShape, 0);
            wallBody->SetUserData((void*) &wm._fieldModel);
        }
    }

    // Vehicle dynamic body
    _vehicleBody = initVehicleBody(wm._vehicleModel);

    // Game piece dynamic bodies
    _gamePieceBodies = initGamePieceBodies(wm._gamePieceModels);

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    _velocityIterations = 6;
    _positionIterations = 2;
}



void CollisionDetector::update(WorldModel& wm, double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;

    // Update the vehicle velocities
    _vehicleBody->SetLinearVelocity(b2Vec2(wm._vehicleModel._state.pose.vx, wm._vehicleModel._state.pose.vy));
    _vehicleBody->SetAngularVelocity(wm._vehicleModel._state.pose.omega);

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

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world.Step(elapsedTime, _velocityIterations, _positionIterations);

    // Update the vehicle model
    {
        b2Vec2 position = _vehicleBody->GetPosition();
        float angle = _vehicleBody->GetAngle();
        b2Vec2 velocity = _vehicleBody->GetLinearVelocity();

        wm._vehicleModel._state.pose.x = position.x;
        wm._vehicleModel._state.pose.y = position.y;
        wm._vehicleModel._state.pose.theta = angle;
    }

    // Update the game piece models
    {
        for (unsigned int i=0; i<_gamePieceBodies.size(); i++)
        {
            b2Vec2 position = _gamePieceBodies[i]->GetPosition();
            b2Vec2 velocity = _gamePieceBodies[i]->GetLinearVelocity();
            float angle = _gamePieceBodies[i]->GetAngle();

            wm._gamePieceModels[i]._state.pose.x = position.x;
            wm._gamePieceModels[i]._state.pose.y = position.y;
        }
    }

    _prevTimestamp = currTimestamp;
}



void CollisionDetector::reset(WorldModel& wm)
{
    // Destroy vehicle and game piece bodies
    _world.DestroyBody(_vehicleBody);
    for (int i=0; i<_gamePieceBodies.size(); i++)
    {
        b2Body* body = _gamePieceBodies.back();
        _gamePieceBodies.pop_back();
        _world.DestroyBody(body);
    }

    // Re-initialize vehicle and game pieces from their models
    _vehicleBody = initVehicleBody(wm._vehicleModel);
    _gamePieceBodies = initGamePieceBodies(wm._gamePieceModels);
}



b2Body* CollisionDetector::initVehicleBody(const VehicleModel& vehicleModel)
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef vehicleBodyDef;
    vehicleBodyDef.type = b2_dynamicBody;
    vehicleBodyDef.position.Set(vehicleModel._state.pose.x, vehicleModel._state.pose.y);
    b2Body* vehicleBody = _world.CreateBody(&vehicleBodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape vehicleDynamicBox;
    Polygon2d bounds = vehicleModel._boundingPolygon;
    b2Vec2 vertices[bounds.numVertices()];
    for (unsigned int i=0; i<bounds.numVertices(); i++)
    {
        Vertex2d v = bounds.vertices().at(i);
        vertices[i] = b2Vec2(v.x, v.y);
    }
    vehicleDynamicBox.Set(vertices, bounds.numVertices());

    // Define the dynamic body fixture.
    b2FixtureDef vehicleFixtureDef;
    vehicleFixtureDef.shape = &vehicleDynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    vehicleFixtureDef.density = 83.91f / 0.53f;

    // Override the default friction.
    vehicleFixtureDef.friction = 0.3f;

    // Add the shape to the body.
    vehicleBody->CreateFixture(&vehicleFixtureDef);

    // Add user data
    vehicleBody->SetUserData((void*) &vehicleModel);

    return vehicleBody;
}



std::vector<b2Body*> CollisionDetector::initGamePieceBodies(const std::vector<GamePieceModel>& gamePieceModels)
{
    std::vector<b2Body*> gamePieceBodies;
    for (const auto& gamePieceModel : gamePieceModels)
    {
        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef gamePieceBodyDef;
        gamePieceBodyDef.type = b2_dynamicBody;
        gamePieceBodyDef.position.Set(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y);
        b2Body* gamePieceBody = _world.CreateBody(&gamePieceBodyDef);

        // Define another box shape for our dynamic body.
        b2CircleShape gamePieceDynamicCircle;
        gamePieceDynamicCircle.m_radius = gamePieceModel._radius;

        b2FixtureDef gamePieceFixtureDef;
        gamePieceFixtureDef.shape = &gamePieceDynamicCircle;
        gamePieceFixtureDef.restitution = 0.5f;

        gamePieceFixtureDef.density = 1.0f;
        gamePieceFixtureDef.friction = 0.3f;
        gamePieceBody->CreateFixture(&gamePieceFixtureDef);
        gamePieceBody->SetUserData((void*) &gamePieceModel);
        gamePieceBodies.push_back(gamePieceBody);
    }

    return gamePieceBodies;
}
