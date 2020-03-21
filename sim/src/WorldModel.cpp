/**
 * Copyright (c) 2020 Team 3260
 */

#include <GamePieceModel.h>
#include "WorldModel.h"


WorldModel::WorldModel(ConfigReader& config, double timestamp) :
        _fieldModel(config, timestamp)
{
    // Initialize all vehicle models
    for (int i=0; i<config.players.size(); i++)
    {
        VehicleModel v(config, timestamp, i);
        _vehicleModels.emplace_back(v);
    }

    // Initialize all game piece models
    for (const auto& initialPosition : config.sim.gamePiece.initialPositions)
    {
        _gamePieceModels.emplace_back(config.sim.gamePiece.radius, initialPosition.x, initialPosition.y);
    }

    _physicsEngine = PhysicsEngine(_fieldModel, _vehicleModels, _gamePieceModels, timestamp);
}



void WorldModel::update(double timestamp)
{
    // Update external forces on field and vehicle
    _fieldModel.update(timestamp);
    for (auto& vehicleModel : _vehicleModels)
    {
        vehicleModel.update(timestamp);
    }

    // Apply collisions and constraints
    _physicsEngine.update(_fieldModel, _vehicleModels, _gamePieceModels, timestamp);
}



void WorldModel::reset()
{
    // Field
    _fieldModel.reset();

    // Vehicle
    for (auto& vehicleModel : _vehicleModels)
    {
        vehicleModel.reset();
    }

    // Game pieces
    for (auto& gamePiece : _gamePieceModels)
    {
        gamePiece.reset();
    }

    // Physics engine
    _physicsEngine.reset(_fieldModel, _vehicleModels, _gamePieceModels);
}



SimState WorldModel::getSimState()
{
    SimState s;

    s.blueScore = std::get<0>(getScore());
    s.redScore = std::get<1>(getScore());

    s.field.inCollision = _fieldModel._inCollision;

    for (const auto& vehicle : _vehicleModels)
    {
        SimState::VehicleState v{};
        v.team = vehicle._team;
        v.alliance = vehicle._alliance;
        v.x = (float) vehicle._state.pose.x;
        v.y = (float) vehicle._state.pose.y;
        v.theta = (float) vehicle._state.pose.theta;
        v.intakeCenterMotorSpeed = (float) vehicle._state.intakeCenterMotorSpeed;
        v.intakeLeftMotorSpeed = (float) vehicle._state.intakeLeftMotorSpeed;
        v.intakeRightMotorSpeed = (float) vehicle._state.intakeRightMotorSpeed;
        v.tubeMotorSpeed = (float) vehicle._state.tubeMotorSpeed;
        s.vehicles.push_back(v);
    }

    for (const auto& gamePiece : _gamePieceModels)
    {
        SimState::GamePieceState g{};
        g.x = (float) gamePiece._state.pose.x;
        g.y = (float) gamePiece._state.pose.y;
        g.z = (float) gamePiece._state.pose.z;
        g.ingestionState = gamePiece._state.ingestion;
        s.gamePieces.push_back(g);
    }

    return s;
}
