/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgDB/ReadFile>
#include "FieldView.h"
#include "ViewUtils.h"
#include "Color.h"


FieldView::FieldView(const ConfigReader& config, const FieldModel& fieldModel)
{
    if (config.debugView)
    {
        _fieldNode = makeField(fieldModel);
        addChild(_fieldNode);

        _fieldBounds = makeFieldBounds(fieldModel);
        addChild(_fieldBounds);

        osg::ref_ptr<osg::Geode> goalRegion = makeGoals(fieldModel);
        addChild(goalRegion);
    }
    else
    {
        _fieldNode = osgDB::readNodeFile(config.sim.assets.fieldModelFile);
        addChild(_fieldNode);
    }
}



void FieldView::update(const FieldModel& fieldModel)
{
    if (_fieldBounds)
    {
        for (int i=0; i<_fieldBounds->getNumDrawables(); i++)
        {
            osg::Drawable* drawable = _fieldBounds->getDrawable(i);
            osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
            osg::Vec4Array* colorArray = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
            colorArray->clear();
            colorArray->push_back(fieldModel._inCollision ? Color(Color::Red, 127) : Color(Color::Green, 127));
            geom->setColorArray(colorArray);
            geom->setColorBinding(osg::Geometry::BIND_OVERALL);
            geom->dirtyBound();
        }
    }
}



osg::ref_ptr<osg::Geode> FieldView::makeField(const FieldModel& fieldModel)
{
    Geometry::Polygon2d exteriorPolygon = fieldModel._exteriorPolygon;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0, 0, 0));
    for (const auto& vertex : exteriorPolygon.vertices())
    {
        vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0));
    }
    Geometry::Vertex2d firstVertex = exteriorPolygon.vertices()[0];
    vertices->push_back(osg::Vec3(firstVertex.x, firstVertex.y, 0));
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeTriangleFan(vertices, Color(Color::Gray, 32));
    geode->addDrawable(geom);

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::makeFieldBounds(const FieldModel& fieldModel)
{
    double defaultHeight = 0.67;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    // Draw exterior bounds
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& edge : fieldModel._exteriorPolygon.edges())
        {
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, 0));
            vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, defaultHeight));
            vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, defaultHeight));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeQuads(vertices, Color(Color::Green, 127));
        geode->addDrawable(geom);
    }

    // Draw interior bounds
    {
        for (const auto& polygon: fieldModel._interiorPolygons)
        {
            osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
            for (const auto& edge : polygon.edges())
            {
                vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, 0));
                vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, 0));
                vertices->push_back(osg::Vec3(edge.b.x, edge.b.y, defaultHeight));
                vertices->push_back(osg::Vec3(edge.a.x, edge.a.y, defaultHeight));
            }
            osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeQuads(vertices, Color(Color::Green, 127));
            geode->addDrawable(geom);
        }
    }

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::makeGoals(const FieldModel& fieldModel)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : fieldModel._blueGoal.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, Color::Blue);
        geode->addDrawable(geom);
    }
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for (const auto& vertex : fieldModel._redGoal.vertices())
        {
            vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
        }
        osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, Color::Red);
        geode->addDrawable(geom);
    }

    return geode;
}
