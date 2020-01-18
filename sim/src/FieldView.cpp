/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgDB/ReadFile>
#include "FieldView.h"
#include "ViewUtils.h"
#include "Color.h"

#define DEFAULT_FIELD_FILE "/home/psahay/field2020.wrl"


FieldView::FieldView(const ConfigReader& config)
{
    _fieldNode = osgDB::readNodeFile(DEFAULT_FIELD_FILE);
    if (_fieldNode)
    {
        addChild(_fieldNode);
    }
    else
    {
        std::cerr << "File not found: " << DEFAULT_FIELD_FILE << std::endl;
        _fieldNode = makeField();
        addChild(_fieldNode);
    }
}



void FieldView::update(const FieldModel& fieldModel)
{

}



osg::ref_ptr<osg::Geode> FieldView::makeField()
{
    float width = 16.49; // 54 ft 1 in
    float depth = 8.10; // 26 ft 7 in
    float height = 0.30; // Random guess

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-width/2, -depth/2, 0));
    vertices->push_back(osg::Vec3(width/2, -depth/2, 0));
    vertices->push_back(osg::Vec3(width/2, depth/2, 0));
    vertices->push_back(osg::Vec3(-width/2, depth/2, 0));
    osg::ref_ptr<osg::Geometry> geom = ViewUtils::makeQuads(vertices, Color(Color::Red, 32));
    geode->addDrawable(geom);

    return geode;
}
