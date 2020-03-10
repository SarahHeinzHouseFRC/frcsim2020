/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include <ConfigReader.h>
#include "FieldView.h"
#include "ViewUtils.h"
#include "Color.h"


FieldView::FieldView(const ConfigReader& config)
{
    if (config.debugView)
    {
        _fieldNode = drawGround(config);
        addChild(_fieldNode);

        _fieldBounds = drawBoundary(config);
        addChild(_fieldBounds);

//        osg::ref_ptr<osg::Geode> goalRegion = drawGoals(config);
//        addChild(goalRegion);
    }
    else
    {
        _fieldNode = osgDB::readNodeFile(config.sim.assets.fieldModelFile);
        addChild(_fieldNode);
    }
}



void FieldView::update(const SimState& simState)
{

}



osg::ref_ptr<osg::Geode> FieldView::drawGround(const ConfigReader& config)
{
    Geometry::Polygon2d exteriorPolygon = config.sim.field.exteriorPolygon;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    double minX=0, maxX=0, minY=0, maxY=0;
    for (const auto& vertex : exteriorPolygon.vertices())
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

    // Draw exterior bounds
    {
        Geometry::Polygon2d exteriorPolygon = config.sim.field.exteriorPolygon;
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& edge : exteriorPolygon.edges())
        {
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, defaultHeight));
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, defaultHeight));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(geom);
    }

    // Draw interior bounds
    {
        const auto& interiorPolygons = config.sim.field.interiorPolygons;
        for (Geometry::Polygon2d polygon : interiorPolygons)
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
    }

    return geode;
}


/*
osg::ref_ptr<osg::Geode> FieldView::drawGoals(const ConfigReader& config)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : fieldModel._blueGoal.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Blue);
        geode->addDrawable(geom);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : fieldModel._redGoal.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::drawLineLoop(vertices, Color::Red);
        geode->addDrawable(geom);
    }

    return geode;
}
*/
