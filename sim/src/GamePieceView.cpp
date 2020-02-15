/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <GamePieceModel.h>
#include "Color.h"
#include "GamePieceView.h"


GamePieceView::GamePieceView(const ConfigReader& config, const GamePieceModel& gamePieceModel)
{
    if (config.debugView)
    {
        _node = makeView(gamePieceModel);
        addChild(_node);
    }
    else
    {
        _node = osgDB::readNodeFile(config.sim.assets.gamePieceModelFile);
        addChild(_node);
    }
    setPosition(osg::Vec3(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y, 0));
}



void GamePieceView::update(const GamePieceModel& gamePieceModel)
{
    setPosition(osg::Vec3(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y, gamePieceModel._radius));

    if (_shape)
    {
        switch (gamePieceModel._state.ingestion)
        {
            case GamePieceModel::NOT_INGESTED:
                _shape->setColor(Color::Yellow);
                break;

            case GamePieceModel::CENTER_INGESTIBLE:
                _shape->setColor(Color::Orange);
                break;

            case GamePieceModel::LEFT_INGESTIBLE:
                _shape->setColor(Color::Blue);
                break;

            case GamePieceModel::RIGHT_INGESTIBLE:
                _shape->setColor(Color::Red);
                break;

            case GamePieceModel::INGESTED:
                _shape->setColor(Color::Green);
                break;
        }
    }
}



osg::ref_ptr<osg::Geode> GamePieceView::makeView(const GamePieceModel& gamePieceModel)
{
    _shape = new osg::ShapeDrawable;
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3d(0, 0, 0), gamePieceModel._radius);
    _shape->setShape(sphere);
    _shape->setColor(Color::Yellow);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(_shape);
    return geode;
}
