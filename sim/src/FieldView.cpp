/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include "FieldView.h"
#include "ViewUtils.h"
#include "Color.h"


FieldView::FieldView(const ConfigReader& config)
{
    if (config.debugView || config.headless)
    {
        _fieldNode = drawGround(config);
        addChild(_fieldNode);

        _fieldBounds = drawBoundary(config);
        addChild(_fieldBounds);

        osg::ref_ptr<osg::Geode> goalRegion = drawGoals(config);
        addChild(goalRegion);

        osg::ref_ptr<osg::Geode> outtakes = drawOuttakes(config);
        addChild(outtakes);
    }
    else
    {
        _fieldNode = osgDB::readNodeFile(config.sim.assets.fieldModelFile);
        addChild(_fieldNode);
    }
}



void FieldView::update(const SimState::FieldState& state)
{
    if (_fieldBounds)
    {
        for (int i=0; i<_fieldBounds->getNumDrawables(); i++)
        {
            osg::Drawable* drawable = _fieldBounds->getDrawable(i);
            osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
            osg::Vec4Array* colorArray = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
            colorArray->clear();
            colorArray->push_back(state.inCollision ? Color(Color::Red, 127) : Color(Color::Green, 127));
            geom->setColorArray(colorArray);
            geom->setColorBinding(osg::Geometry::BIND_OVERALL);
            geom->dirtyBound();
        }
    }
}



osg::ref_ptr<osg::Geode> FieldView::drawGround(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    double minX=0, maxX=0, minY=0, maxY=0;
    for (const auto& vertex : config.sim.field.exteriorWall.vertices())
    {
        if (minX > vertex.x) { minX = vertex.x; }
        if (maxX < vertex.x) { maxX = vertex.x; }
        if (minY > vertex.y) { minY = vertex.y; }
        if (maxY < vertex.y) { maxY = vertex.y; }
    }
    minX -= 0.25;
    minY -= 0.25;
    maxX += 0.25;
    maxY += 0.25;
    vertices->push_back(osg::Vec3(minX, minY, 0));
    vertices->push_back(osg::Vec3(maxX, minY, 0));
    vertices->push_back(osg::Vec3(maxX, maxY, 0));
    vertices->push_back(osg::Vec3(minX, maxY, 0));
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawQuads(vertices, Color(Color::Gray, 32));
    geode->addDrawable(geom);

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::drawBoundary(const ConfigReader& config)
{
    double defaultHeight = 0.67;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    std::vector<Geometry::Polygon2d> fieldPolygons {
            config.sim.field.exteriorWall,
            config.sim.field.rightTrenchRightWall,
            config.sim.field.rightTrenchLeftWall,
            config.sim.field.leftTrenchRightWall,
            config.sim.field.leftTrenchLeftWall,
            config.sim.field.rightColumn,
            config.sim.field.topColumn,
            config.sim.field.leftColumn,
            config.sim.field.bottomColumn,
    };

    // Draw field interior and exterior polygons
    for (const auto& polygon : fieldPolygons)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& edge : polygon.edges())
        {
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, defaultHeight));
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, defaultHeight));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(geom);
    }

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::drawGoals(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    std::vector<std::tuple<Geometry::Polygon2d, Color>> goalRegions {
            { config.sim.field.blueGoalRegion, Color::Blue },
            { config.sim.field.redGoalRegion, Color::Red },
    };

    for (const auto& [polygon, color] : goalRegions)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : polygon.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, color);
        geode->addDrawable(geom);
    }

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::drawOuttakes(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(config.sim.field.blueOuttake.x - 0.1, config.sim.field.blueOuttake.y - 0.01, 0));
        vertices->push_back(osg::Vec3(config.sim.field.blueOuttake.x + 0.1, config.sim.field.blueOuttake.y - 0.01, 0));
        vertices->push_back(osg::Vec3(config.sim.field.blueOuttake.x + 0.1, config.sim.field.blueOuttake.y - 0.01, 0.2));
        vertices->push_back(osg::Vec3(config.sim.field.blueOuttake.x - 0.1, config.sim.field.blueOuttake.y - 0.01, 0.2));
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Blue);
        geode->addDrawable(geom);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(config.sim.field.redOuttake.x - 0.1, config.sim.field.redOuttake.y + 0.01, 0));
        vertices->push_back(osg::Vec3(config.sim.field.redOuttake.x + 0.1, config.sim.field.redOuttake.y + 0.01, 0));
        vertices->push_back(osg::Vec3(config.sim.field.redOuttake.x + 0.1, config.sim.field.redOuttake.y + 0.01, 0.2));
        vertices->push_back(osg::Vec3(config.sim.field.redOuttake.x - 0.1, config.sim.field.redOuttake.y + 0.01, 0.2));
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Red);
        geode->addDrawable(geom);
    }

    return geode;
}
