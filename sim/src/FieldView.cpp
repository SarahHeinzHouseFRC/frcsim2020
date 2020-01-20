/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgDB/ReadFile>
#include "FieldView.h"
#include "ViewUtils.h"
#include "Color.h"

#define DEFAULT_FIELD_FILE "/home/psahay/field2020.wrl"


FieldView::FieldView(const ConfigReader& config, const FieldModel& fieldModel)
{
    if (config.debugView)
    {
        _fieldNode = makeField(fieldModel);
        addChild(_fieldNode);
    }
    else
    {
        _fieldNode = osgDB::readNodeFile(DEFAULT_FIELD_FILE);
        addChild(_fieldNode);
    }

    _fieldBounds = makeFieldBounds(fieldModel);
    addChild(_fieldBounds);
}



void FieldView::update(const FieldModel& fieldModel)
{
    osg::Drawable* drawable = _fieldBounds->getDrawable(0);
    osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
    osg::Vec4Array* colorArray = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
    colorArray->clear();
    colorArray->push_back(fieldModel._inCollision ? Color::Red : Color::Green);
    geom->setColorArray(colorArray);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    geom->dirtyBound();
}



osg::ref_ptr<osg::Geode> FieldView::makeField(const FieldModel& fieldModel)
{
    Geometry::Polygon2d fieldBounds = fieldModel.exteriorPolygon().boundingBox();

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    for (const auto& vertex : fieldBounds.vertices())
    {
        vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0));
    }
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeQuads(vertices, Color(Color::Gray, 32));
    geode->addDrawable(geom);

    return geode;
}



osg::ref_ptr<osg::Geode> FieldView::makeFieldBounds(const FieldModel& fieldModel)
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    for (const auto& vertex : fieldModel.exteriorPolygon().vertices())
    {
        vertices->push_back(osg::Vec3(vertex.x, vertex.y, 0.1));
    }
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeLineLoop(vertices, (fieldModel._inCollision ? Color::Red : Color::Green));
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom);
    return geode;
}
