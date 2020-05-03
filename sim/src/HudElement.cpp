/**
 * Copyright (c) 2020 Team 3260
 */

#include "ViewUtils.h"
#include "HudElement.h"


HudLabel::HudLabel(const std::string& text, float x, float y, float fontSize, const std::string& fontFile) :
        _x(x), _y(y)
{
    this->setText(text);
    this->setCharacterSize(fontSize);
    this->setPosition(osg::Vec3(x, y, 0));
    this->setFont(fontFile);
    this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    this->setDataVariance(osg::Object::DYNAMIC);
}



HudBox::HudBox(float x, float y, float width, float height, const Color& color) :
    _x(x), _y(y), _width(width), _height(height)
{
    _vertices = new osg::Vec3Array;
    _vertices->push_back(osg::Vec3(_x, _y, -0.01));
    _vertices->push_back(osg::Vec3(_x + _width, _y, -0.01));
    _vertices->push_back(osg::Vec3(_x + _width, _y + _height, -0.01));
    _vertices->push_back(osg::Vec3(_x, _y + _height, -0.01));
    _colors = new osg::Vec4Array;
    _colors->push_back(color);

    this->setVertexArray(_vertices);
    this->setColorArray(_colors);
    this->setColorBinding(osg::Geometry::BIND_OVERALL);
    this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // Turn off lighting
    this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); // Turn on blending
    this->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, _vertices->size())); // Set geometry type
}



void HudBox::onWindowResize(int windowWidth, int windowHeight)
{
    _vertices = new osg::Vec3Array;
    _vertices->push_back(osg::Vec3(_x, windowHeight + _y, -0.01));
    _vertices->push_back(osg::Vec3(_x + _width, windowHeight + _y, -0.01));
    _vertices->push_back(osg::Vec3(_x + _width, windowHeight + _y + _height, -0.01));
    _vertices->push_back(osg::Vec3(_x, windowHeight + _y + _height, -0.01));
    this->setVertexArray(_vertices);
}
