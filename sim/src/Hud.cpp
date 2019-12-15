/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include <RobotModel.h>
#include "Color.h"
#include "Hud.h"

#define RADS_PER_SEC_TO_RPM 9.5492


Hud::Hud(const ConfigReader& config) : _width(225)
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
    _root->addChild(_geode);

    // Add labels to _labelsGeode
    _labelsGeode = new osg::Geode;
    _root->addChild(_labelsGeode);
    _connected = new TopLabel("Connected", -94);
    _connected->setDrawMode(osgText::TextBase::TEXT | osgText::TextBase::FILLEDBOUNDINGBOX);
    _connected->setBoundingBoxMargin(5);
    _labelsGeode->addChild(_connected);

    auto title = new TopLabel("Robot Sim", -34, 24);
    _labelsGeode->addChild(title);

    auto subtitle = new TopLabel("Team SHARP 3260", -54);
    subtitle->setColor(osg::Vec4(0.8, 0.8, 1, 1));
    _labelsGeode->addChild(subtitle);

    _robotState = new TopLabel("", -130);
    _labelsGeode->addChild(_robotState);
}



void Hud::onWindowResize(int width, int height)
{
    _vertices->clear();
    _vertices->push_back(osg::Vec3(0, 0, -1));
    _vertices->push_back(osg::Vec3(_width, 0, -1));
    _vertices->push_back(osg::Vec3(_width, height, -1));
    _vertices->push_back(osg::Vec3(0, height, -1));
    _geom->setVertexArray(_vertices);

    for (unsigned int i=0; i<_labelsGeode->getNumChildren(); i++)
    {
        osg::Node* node = _labelsGeode->getChild(i);
        Label* label = dynamic_cast<Label*>(node);
        label->onWindowResize(width, height);
    }
}



void Hud::displayConnected(bool isConnected)
{
    if (isConnected)
    {
        _connected->setBoundingBoxColor(Color::Green);
        _connected->setText("Connected");
    }
    else
    {
        _connected->setBoundingBoxColor(Color::Red);
        _connected->setText("Disconnected");
    }
}



void Hud::displayRobotState(const RobotModel& robotModel)
{
    char tmp[1024];
    sprintf(tmp, "Elevator: \n    Position: %.2f m \n    Motor speed: %.0f RPM", robotModel._state.elevatorCarriagePos, robotModel._state.elevatorMotorSpeed*RADS_PER_SEC_TO_RPM);
    _robotState->setText(tmp);
}
