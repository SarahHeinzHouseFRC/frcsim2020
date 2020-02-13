/**
 * Copyright (c) 2020 Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include <VehicleModel.h>
#include "Color.h"
#include "Hud.h"

#define RADS_PER_SEC_TO_RPM 9.5492
#define RADS_TO_DEG 57.2958
#define M_TO_FT 3.28084


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
    colors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.6));
    _geom->setColorArray(colors, osg::Array::BIND_OVERALL);
    _geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));
    _geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    _geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    _geom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _geode->addDrawable(_geom);
    _root->addChild(_geode);

    constexpr int H1_FONT_SIZE = 24;
    constexpr int H2_FONT_SIZE = 22;
    constexpr int P_FONT_SIZE = 16;

    // Add labels to _labelsGeode
    _labelsGeode = new osg::Geode;
    _root->addChild(_labelsGeode);
    _connected = new TopLabel("Connected", -94, P_FONT_SIZE, config.sim.assets.fontFile);
    _connected->setDrawMode(osgText::TextBase::TEXT | osgText::TextBase::FILLEDBOUNDINGBOX);
    _connected->setBoundingBoxMargin(5);
    _labelsGeode->addChild(_connected);

    float height = 0;

    height -= 34;
    auto title = new TopLabel("Robot Sim", height, H1_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(title);

    height -= 20;
    auto subtitle = new TopLabel("Team SHARP 3260", height, P_FONT_SIZE, config.sim.assets.fontFile);
    subtitle->setColor(osg::Vec4(0.8, 0.8, 1, 1));
    _labelsGeode->addChild(subtitle);

    height -= 100;
    _timer = new TopLabel("00:00", height, H2_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_timer);

    height -= 30;
    _numCollisions = new TopLabel("Collisions: 0", height, H2_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_numCollisions);

    height -= 50;
    _vehiclePoseState = new TopLabel("", height, P_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_vehiclePoseState);

    height -= 60;
    _vehicleDrivetrainState = new TopLabel("", height, P_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_vehicleDrivetrainState);

    _labelsGeode->addChild(new BottomLabel("[1] Top-down view", 40, P_FONT_SIZE, config.sim.assets.fontFile));
    _labelsGeode->addChild(new BottomLabel("[2] Orbit view", 20, P_FONT_SIZE, config.sim.assets.fontFile));
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



void Hud::displayConnectionStatus(bool isConnected)
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



void Hud::displayVehicleState(const VehicleModel& vehicleModel)
{
    char tmp[1024];

    sprintf(tmp, "Pose: \n    X: %.1f ft \n    Y: %.1f ft \n    Theta: %.0f deg", vehicleModel._state.pose.x*M_TO_FT, vehicleModel._state.pose.y*M_TO_FT, vehicleModel._state.pose.theta*RADS_TO_DEG);
    _vehiclePoseState->setText(tmp);

    sprintf(tmp, "Drivetrain: \n    Left motor: %.0f RPM \n    Right motor: %.0f RPM", vehicleModel._state.leftDriveMotorSpeed*RADS_PER_SEC_TO_RPM, vehicleModel._state.rightDriveMotorSpeed*RADS_PER_SEC_TO_RPM);
    _vehicleDrivetrainState->setText(tmp);
}



void Hud::displayTimerStatus(bool running, double timerValue)
{
    if (timerValue <= 0)
    {
        _timer->setColor(Color::Red);
    }
    else if (timerValue <= 10)
    {
        _timer->setColor(Color::Yellow);
    }
    else if (!running)
    {
        _timer->setColor(Color::Gray);
    }
    else
    {
        _timer->setColor(Color::White);
    }
    char tmp[1024];
    sprintf(tmp, "Timer: %d:%02d", (int) timerValue / 60, (int) timerValue % 60);
    _timer->setText(tmp);
}



void Hud::displayNumCollisions(int numCollisions)
{
    char tmp[1024];
    sprintf(tmp, "Collisions: %d", numCollisions);
    _numCollisions->setText(tmp);
}
