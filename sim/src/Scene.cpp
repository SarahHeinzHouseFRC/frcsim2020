/**
 * Copyright (c) 2020 Team 3260
 */

#include "Scene.h"


Scene::Scene(const ConfigReader& config) : _showLidar(true)
{
    _root = new osg::Group;

    _fieldView = new FieldView(config);
    _root->addChild(_fieldView);

    for (int i=0; i<config.players.size(); i++)
    {
        osg::ref_ptr<VehicleView> vehicleView = new VehicleView(config, i);
        _vehicleViews.push_back(vehicleView);
        _root->addChild(vehicleView);
    }

    for (int i=0; i<config.sim.gamePiece.initialPositions.size(); i++)
    {
        osg::ref_ptr<GamePieceView> gamePieceView = new GamePieceView(config, i);
        _root->addChild(gamePieceView);
        _gamePieceViews.push_back(gamePieceView);
    }
}



void Scene::update(const SimState& simState, bool showLidar)
{
    // Update field
    _fieldView->update(simState.field);

    // Update vehicles
    for (int i=0; i<simState.vehicles.size(); i++)
    {
        _vehicleViews.at(i)->update(simState.vehicles.at(i), showLidar);
    }

    // Update game pieces
    for (int i=0; i<simState.gamePieces.size(); i++)
    {
        _gamePieceViews.at(i)->update(simState.gamePieces.at(i));
    }
}
