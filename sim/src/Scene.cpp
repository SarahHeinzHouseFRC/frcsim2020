/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "ConfigReader.h"
#include "Scene.h"


Scene::Scene(const ConfigReader& config, const WorldModel& wm)
{
    _root = new osg::Group;

    _vehicleView = new VehicleView(config, wm._vehicleModel);
    _root->addChild(_vehicleView);

    _fieldView = new FieldView(config, wm._fieldModel);
    _root->addChild(_fieldView);

    _gamePieceView = new GamePieceView(config, wm._gamePieceModel);
    _root->addChild(_gamePieceView);
}



void Scene::update(const WorldModel& wm)
{
    _vehicleView->update(wm._vehicleModel);
    _fieldView->update(wm._fieldModel);
    _gamePieceView->update(wm._gamePieceModel);
}
