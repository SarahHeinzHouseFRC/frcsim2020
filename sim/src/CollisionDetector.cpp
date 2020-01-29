/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <VehicleModel.h>
#include "CollisionDetector.h"

using namespace Geometry;


CollisionDetector::CollisionDetector(const WorldModel& wm, double timestamp) :
        _prevTimestamp(timestamp),
        _gravity(0.0f, 0.0f),
        _world(_gravity)
{
    //
    // Define the field using edges
    //

    // Exterior polygon
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
    }

    // Interior polygons
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
        }
    }

    {
        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef vehicleBodyDef;
        vehicleBodyDef.type = b2_dynamicBody;
        vehicleBodyDef.position.Set(wm._vehicleModel._state.pose.x, wm._vehicleModel._state.pose.y);
        _vehicleBody = _world.CreateBody(&vehicleBodyDef);

        // Define another box shape for our dynamic body.
        b2PolygonShape vehicleDynamicBox;
        Polygon2d bounds = wm._vehicleModel._boundingPolygon;
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
        vehicleFixtureDef.density = 1.0f;

        // Override the default friction.
        vehicleFixtureDef.friction = 0.3f;

        // Add the shape to the body.
        _vehicleBody->CreateFixture(&vehicleFixtureDef);
    }

    {
        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef gamePieceBodyDef;
        gamePieceBodyDef.type = b2_dynamicBody;
        gamePieceBodyDef.position.Set(0.0f, 5.0f);
        _gamePieceBody = _world.CreateBody(&gamePieceBodyDef);

        // Define another box shape for our dynamic body.
        b2CircleShape gamePieceDynamicCircle;
        gamePieceDynamicCircle.m_radius = wm._gamePieceModel._radius;

        b2FixtureDef gamePieceFixtureDef;
        gamePieceFixtureDef.shape = &gamePieceDynamicCircle;
        gamePieceFixtureDef.restitution = 0.5f;

        gamePieceFixtureDef.density = 10.0f;
        gamePieceFixtureDef.friction = 0.3f;
        _gamePieceBody->CreateFixture(&gamePieceFixtureDef);
    }

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    _velocityIterations = 6;
    _positionIterations = 2;
}



void CollisionDetector::detectCollisions(WorldModel& wm, double currTimestamp)
{
    double elapsedTime = currTimestamp - _prevTimestamp;

//    float vx = 0, vy = 0, omega = 0;
//    if (wm._vehicleModel._state.pose.omega > 0) omega = 0.5;
//    if (wm._vehicleModel._state.pose.omega < 0) omega = -0.5;
//    if (wm._vehicleModel._state.pose.vx > 0) vx = 0.5 * cos(wm._vehicleModel._state.pose.theta);
//    if (wm._vehicleModel._state.pose.vx < 0) vx = -0.5 * cos(wm._vehicleModel._state.pose.theta);
//    if (wm._vehicleModel._state.pose.vy > 0) vy = 0.5 * sin(wm._vehicleModel._state.pose.theta);
//    if (wm._vehicleModel._state.pose.vy < 0) vy = -0.5 * sin(wm._vehicleModel._state.pose.theta);
//    _vehicleBody->SetLinearVelocity(b2Vec2(vx, vy));
//    _vehicleBody->SetAngularVelocity(omega);
//    b2Vec2 pos = _vehicleBody->GetWorldCenter();
//    _vehicleBody->ApplyForce({ vx/10, vy/10 }, pos, true);
//    _vehicleBody->ApplyTorque(omega/100, true);
//    b2Vec2 currentVel = _vehicleBody->GetLinearVelocity();
//    b2Vec2 desiredVel = b2Vec2(wm._vehicleModel._state.pose.vx, wm._vehicleModel._state.pose.vy);
//    b2Vec2 impulse = desiredVel - currentVel;
//    impulse *= _vehicleBody->GetMass();
//    _vehicleBody->ApplyLinearImpulse(impulse, _vehicleBody->GetWorldCenter(), true);
//
//    float currentOmega = _vehicleBody->GetAngularVelocity();
//    float desiredOmega = wm._vehicleModel._state.pose.omega;
//    float rotImpulse = (desiredOmega - currentOmega) * _vehicleBody->GetInertia();
//    _vehicleBody->ApplyAngularImpulse(rotImpulse, true);
//
//    printf("Current vel = %f, %f | Desired vel = %f, %f | Current omega = %f | Desired omega = %f\n", currentVel.x, currentVel.y, desiredVel.x, desiredVel.y, currentOmega, desiredOmega);

    _vehicleBody->SetLinearVelocity(b2Vec2(wm._vehicleModel._state.pose.vx, wm._vehicleModel._state.pose.vy));
    _vehicleBody->SetAngularVelocity(wm._vehicleModel._state.pose.omega);

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world.Step(elapsedTime, _velocityIterations, _positionIterations);

    // Now print the position and angle of the body.
    {
        b2Vec2 position = _vehicleBody->GetPosition();
        float angle = _vehicleBody->GetAngle();
        b2Vec2 velocity = _vehicleBody->GetLinearVelocity();

        wm._vehicleModel._state.pose.x = position.x;
        wm._vehicleModel._state.pose.y = position.y;
        wm._vehicleModel._state.pose.theta = angle;
//        wm._vehicleModel._state.pose.vx = velocity.x;
//        wm._vehicleModel._state.pose.vy = velocity.y;
//        printf("Vehicle _vx, _vy, _state.pose.omega = %f, %f, %f | Vel: %f, %f\n", wm._vehicleModel._state.pose.vx, wm._vehicleModel._state.pose.vy, wm._vehicleModel._state.pose.omega, velocity.x, velocity.y);
    }

    {
        b2Vec2 position = _gamePieceBody->GetPosition();
        b2Vec2 velocity = _gamePieceBody->GetLinearVelocity();
        float angle = _gamePieceBody->GetAngle();

        wm._gamePieceModel._state.pose.x = position.x;
        wm._gamePieceModel._state.pose.y = position.y;
//        wm._gamePieceModel._state.pose.vx = velocity.x;
//        wm._gamePieceModel._state.pose.vy = velocity.y;
    }

    _prevTimestamp = currTimestamp;
}
