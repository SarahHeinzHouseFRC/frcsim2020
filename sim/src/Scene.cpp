/**
 * Copyright (c) 2020 Team 3260
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

    for (const auto& gamePieceModel : wm._gamePieceModels)
    {
        osg::ref_ptr<GamePieceView> gamePieceView = new GamePieceView(config, gamePieceModel);
        _root->addChild(gamePieceView);
        _gamePieceViews.push_back(gamePieceView);
    }
}



void Scene::update(const WorldModel& wm)
{
    _vehicleView->update(wm._vehicleModel);
    _fieldView->update(wm._fieldModel);
    for (unsigned int i=0; i<wm._gamePieceModels.size(); i++)
    {
        _gamePieceViews[i]->update(wm._gamePieceModels[i]);
    }
}
