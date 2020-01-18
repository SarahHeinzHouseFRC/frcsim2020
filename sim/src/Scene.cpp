/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "ConfigReader.h"
#include "Scene.h"


Scene::Scene(const ConfigReader& config)
{
    _root = new osg::Group;

    _vehicleView = new VehicleView(config);
    _root->addChild(_vehicleView);

    _fieldView = new FieldView(config);
    _root->addChild(_fieldView);
}



void Scene::update(const VehicleModel& vehicleModel, const FieldModel& fieldModel)
{
    _vehicleView->update(vehicleModel);
    _fieldView->update(fieldModel);
}
