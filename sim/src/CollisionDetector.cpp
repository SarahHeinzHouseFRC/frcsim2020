/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "CollisionDetector.h"

using namespace Geometry;


CollisionDetector::CollisionDetector() : _gravity(0.0f, -10.0f), _world(_gravity)
{
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = _world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f);
    _body = _world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    // Add the shape to the body.
    _body->CreateFixture(&fixtureDef);

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    _timeStep = 1.0f / 60.0f;
    _velocityIterations = 6;
    _positionIterations = 2;
}



void CollisionDetector::detectCollisions(FieldModel& fieldModel, VehicleModel& vehicleModel, GamePieceModel& gamePieceModel)
{
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world.Step(_timeStep, _velocityIterations, _positionIterations);

    // Now print the position and angle of the body.
    b2Vec2 position = _body->GetPosition();
    float angle = _body->GetAngle();

    gamePieceModel._state.pose.x = position.x;
    gamePieceModel._state.pose.y = position.y;
}
