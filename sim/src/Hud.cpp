/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include "Hud.h"


Hud::Hud(const ConfigReader& config) : _width(250)
{
    _camera = new osg::Camera;

    _camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 720));
    _camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    _camera->setViewMatrix(osg::Matrix::identity());
    _camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    _camera->setRenderOrder(osg::Camera::POST_RENDER);
    _camera->setAllowEventFocus(false);

    _root = new osg::Group;
    _camera->addChild(_root);

    // Make a background
    _geode = new osg::Geode;
    _geom = new osg::Geometry;
    _vertices = new osg::Vec3Array;
    _vertices->push_back(osg::Vec3(0, 0, -1));
    _vertices->push_back(osg::Vec3(_width, 0, -1));
    _vertices->push_back(osg::Vec3(_width, 720, -1));
    _vertices->push_back(osg::Vec3(0, 720, -1));
    _geom->setVertexArray(_vertices);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1, 1, 1, 0.1));
    _geom->setColorArray(colors, osg::Array::BIND_OVERALL);
    _geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));
    _geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    _geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    _geom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _geode->addDrawable(_geom);

    osg::ref_ptr<Label> title = new Label("Robot Sim", Label::FROM_TOP, osg::Vec3(15, -34, 0));
    title->setCharacterSize(24);
    addLabel(title);

    osg::ref_ptr<Label> subtitle = new Label("Team SHARP 3260", Label::FROM_TOP, osg::Vec3(15, -54, 0));
    subtitle->setCharacterSize(16);
    subtitle->setColor(osg::Vec4(0.8, 0.8, 1, 1));
    addLabel(subtitle);

    _root->addChild(_geode);
}



void Hud::addLabel(osg::ref_ptr<Label> label)
{
    _geode->addDrawable(label);
    _labels.push_back(label);
}



void Hud::onWindowResize(int width, int height)
{
    _vertices->clear();
    _vertices->push_back(osg::Vec3(0, 0, -1));
    _vertices->push_back(osg::Vec3(_width, 0, -1));
    _vertices->push_back(osg::Vec3(_width, height, -1));
    _vertices->push_back(osg::Vec3(0, height, -1));
    _geom->setVertexArray(_vertices);

    for (auto label : _labels)
    {
        label->updatePosition(width, height);
    }
}



Hud::Label::Label(const std::string& text, Alignment a, const osg::Vec3& pos) :
        _desiredPos(pos), _alignment(a)
{
    this->setText(text);
    this->setPosition(pos);
    this->setFont("/data/fonts/fira-mono.ttf");
    this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    this->setDataVariance(osg::Object::DYNAMIC);
}



void Hud::Label::updatePosition(int windowWidth, int windowHeight)
{
    switch (_alignment)
    {
        case FROM_TOP:
            setPosition(_desiredPos + osg::Vec3(0, windowHeight, 0));
            break;

        case FROM_MIDDLE:
            setPosition(_desiredPos + osg::Vec3(0, windowHeight/2, 0));
            break;

        case FROM_BOTTOM:
            setPosition(_desiredPos);
            break;

    }
}
