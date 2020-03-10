/**
 * Copyright (c) 2020 Team 3260
 */

#include <iostream>
#include "ConfigReader.h"
#include "Scene.h"


#define NUM_VEHICLES 1
#define NUM_GAME_PIECES 20


Scene::Scene(const ConfigReader& config) : _config(config)
{
    _root = new osg::Group;

    for (int i=0; i<NUM_VEHICLES; i++)
    {
        osg::ref_ptr<VehicleView> v = new VehicleView(config);
        _vehicleViews.push_back(v);
        _root->addChild(v);
    }

    _fieldView = new FieldView(config);
    _root->addChild(_fieldView);

    for (int i=0; i<NUM_GAME_PIECES; i++)
    {
        osg::ref_ptr<GamePieceView> g = new GamePieceView(config);
        _gamePieceViews.push_back(g);
        _root->addChild(g);
    }
}



void Scene::update(const SimState& simState)
{
    // Make sure we have enough vehicles
    int diff = simState.vehicles.size() - _vehicleViews.size();
    if (diff > 0)
    {
        for (int i=0; i<diff; i++)
        {
            osg::ref_ptr<VehicleView> v = new VehicleView(_config);
            _vehicleViews.push_back(v);
            _root->addChild(v);
        }
    }
    if (diff < 0)
    {
        for (int i=0; i<-diff; i++)
        {
            _vehicleViews.erase(_vehicleViews.begin());
            _root->removeChild(0, 1);
        }
    }

    // Make sure we have enough game pieces
    if (simState.gamePieces.size() > _gamePieceViews.size())
    {
        for (int i=0; i<simState.gamePieces.size() - _gamePieceViews.size(); i++)
        {
            osg::ref_ptr<GamePieceView> g = new GamePieceView(_config);
            _gamePieceViews.push_back(g);
            _root->addChild(g);
        }
    }


    // Update vehicles
    for (int i=0; i<simState.vehicles.size(); i++)
    {
        _vehicleViews.at(i)->update(simState.vehicles.at(i));
    }

    // Update game pieces
    for (int i=0; i<simState.gamePieces.size(); i++)
    {
        _gamePieceViews.at(i)->update(simState.gamePieces.at(i));
    }
}
