/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Shape>
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
}



osg::ref_ptr<osg::Geode> GamePieceView::makeView(const GamePieceModel& gamePieceModel)
{
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable;
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3d(0, 0, 0), gamePieceModel._radius);
    shape->setShape(sphere);
    shape->setColor(Color::Yellow);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(shape);
    return geode;
}
