/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <ConfigReader.h>
#include "Color.h"
#include "GamePieceView.h"


GamePieceView::GamePieceView(const ConfigReader& config) : _radius(config.sim.gamePiece.radius)
{
    if (config.debugView)
    {
        _gamePieceNode = drawGamePiece(config);
        addChild(_gamePieceNode);
    }
    else
    {
        _gamePieceNode = osgDB::readNodeFile(config.sim.assets.gamePieceModelFile);
        addChild(_gamePieceNode);
    }
}



void GamePieceView::update(const SimState::GamePieceState& state)
{
    setPosition(osg::Vec3(state.x, state.y, state.z + _radius));
}



osg::ref_ptr<osg::Geode> GamePieceView::drawGamePiece(const ConfigReader& config)
{
    _shape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(0, 0, 0), config.sim.gamePiece.radius));
    _shape->setColor(Color::Yellow);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(_shape);
    return geode;
}
