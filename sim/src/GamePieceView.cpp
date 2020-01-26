/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include "Color.h"
#include "GamePieceView.h"

#define DEFAULT_GAME_PIECE_FILE "/home/psahay/ball.wrl"


GamePieceView::GamePieceView(const ConfigReader& config, const GamePieceModel& gamePieceModel)
{
    if (config.debugView)
    {
        _node = makeView(gamePieceModel);
        addChild(_node);
    }
    else
    {
        _node = osgDB::readNodeFile(DEFAULT_GAME_PIECE_FILE);
        addChild(_node);
    }
    setPosition(osg::Vec3(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y, 0));
}



void GamePieceView::update(const GamePieceModel& gamePieceModel)
{
    setPosition(osg::Vec3(gamePieceModel._state.pose.x, gamePieceModel._state.pose.y, 0));
}



osg::ref_ptr<osg::Geode> GamePieceView::makeView(const GamePieceModel& gamePieceModel)
{
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable;
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3d(0, 0, gamePieceModel._radius), gamePieceModel._radius);
    shape->setShape(sphere);
    shape->setColor(Color::Orange);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shape);
    return geode;
}
