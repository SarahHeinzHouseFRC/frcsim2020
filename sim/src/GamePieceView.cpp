/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include "Color.h"
#include "GamePieceModel.h"
#include "GamePieceView.h"


GamePieceView::GamePieceView(const ConfigReader& config, int id) : _radius(config.sim.gamePiece.radius)
{
    if (config.debugView || config.headless)
    {
        _gamePieceNode = drawGamePiece(config);
        addChild(_gamePieceNode);
    }
    else
    {
        _gamePieceNode = osgDB::readNodeFile(config.sim.assets.gamePieceModelFile);
        addChild(_gamePieceNode);
    }
    setPosition(osg::Vec3(config.sim.gamePiece.initialPositions.at(id).x, config.sim.gamePiece.initialPositions.at(id).y, 0));
}



void GamePieceView::update(const SimState::GamePieceState& state)
{
    setPosition(osg::Vec3(state.x, state.y, state.z + _radius));

    if (_shape)
    {
        switch (state.ingestionState)
        {
            case GamePieceModel::NOT_INGESTED:
                _shape->setColor(Color::Yellow);
                break;

            case GamePieceModel::CENTER_INTAKE:
            case GamePieceModel::LEFT_INTAKE:
            case GamePieceModel::RIGHT_INTAKE:
                _shape->setColor(Color::Orange);
                break;

            case GamePieceModel::TUBE:
                _shape->setColor(Color::Green);
                break;

            case GamePieceModel::BLUE_LOW_GOAL:
                _shape->setColor(Color::Blue);
                break;

            case GamePieceModel::RED_LOW_GOAL:
                _shape->setColor(Color::Red);
                break;
        }
    }
}



osg::ref_ptr<osg::Geode> GamePieceView::drawGamePiece(const ConfigReader& config)
{
    _shape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(0, 0, 0), config.sim.gamePiece.radius));
    _shape->setColor(Color::Yellow);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(_shape);
    return geode;
}
