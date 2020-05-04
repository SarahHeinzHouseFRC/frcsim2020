/**
 * Copyright (c) 2020 Team 3260
 */

#include <tuple>
#include <osg/Geode>
#include <osg/Geometry>
#include "VehicleModel.h"
#include "Color.h"
#include "Hud.h"

#define RADS_PER_SEC_TO_RPM 9.5492
#define RADS_TO_DEG 57.2958
#define M_TO_FT 3.28084


Hud::Hud(const ConfigReader& config, int playerId) : _width(225)
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

    float y = 0;

    y -= 34;
    auto title = new HudTopLabel("FRC Sim 2020", _padding, y, H1_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(title);

    y -= 20;
    auto subtitle = new HudTopLabel("Team SHARP 3260", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile);
    subtitle->setColor(osg::Vec4(0.8, 0.8, 1, 1));
    _labelsGeode->addChild(subtitle);

    y -= 10;

    // Are we showing connected status just for one player or for all of them?
    int numPlayers = (playerId >= 0) ? 1 : config.players.size();
    for (int i=0; i<numPlayers; i++)
    {
        y -= 35;
        constexpr float margin = 12.5;
        osg::ref_ptr<HudLabel> label = new HudTopLabel("Connected", _padding + margin, y + margin, P_FONT_SIZE, config.sim.assets.fontFile);
        label->setAlignment(osgText::TextBase::LEFT_CENTER);
        _connectedLabels.push_back(label);
        _labelsGeode->addChild(label);
        osg::ref_ptr<HudBox> labelBoundingBox = new HudBox(_padding, y, _width - 2 * _padding, margin*2, Color::Green);
        _connectedBoxes.push_back(labelBoundingBox);
        _labelsGeode->addChild(labelBoundingBox);
    }

    y -= 50;
    osg::ref_ptr<HudBox> yellowScoreboard = new HudBox(_padding, y, _width - 2 * _padding, P_FONT_SIZE * 2, Color(Color::Yellow, 127));
    _labelsGeode->addDrawable(yellowScoreboard);

    _timer = new HudTopLabel("00:00", _width - 2*_padding, y + P_FONT_SIZE, P_FONT_SIZE, config.sim.assets.fontFile);
    _timer->setAlignment(osgText::TextBase::RIGHT_CENTER);
    _labelsGeode->addChild(_timer);

    osg::ref_ptr<HudTopLabel> matchName = new HudTopLabel("Qualification", 2*_padding, y + P_FONT_SIZE, P_FONT_SIZE, config.sim.assets.fontFile);
    matchName->setAlignment(osgText::TextBase::LEFT_CENTER);
    matchName->setColor(Color::Black);
    _labelsGeode->addChild(matchName);

    y -= 50;
    osg::ref_ptr<HudBox> blueScoreboard = new HudBox(_padding, y, _width - 2 * _padding, H2_FONT_SIZE * 2, Color(Color::Blue, 127));
    _labelsGeode->addDrawable(blueScoreboard);

    _blueScore = new HudTopLabel("0", _width/2, y + H2_FONT_SIZE, H2_FONT_SIZE, config.sim.assets.fontFile);
    _blueScore->setAlignment(osgText::TextBase::CENTER_CENTER);
    _blueScore->setBoundingBoxMargin(10);
    _labelsGeode->addChild(_blueScore);

    y -= 50;
    osg::ref_ptr<HudBox> redScoreboard = new HudBox(_padding, y, _width - 2 * _padding, H2_FONT_SIZE * 2, Color(Color::Red, 127));
    _labelsGeode->addDrawable(redScoreboard);

    _redScore = new HudTopLabel("0", _width/2, y + H2_FONT_SIZE, H2_FONT_SIZE, config.sim.assets.fontFile);
    _redScore->setAlignment(osgText::TextBase::CENTER_CENTER);
    _redScore->setBoundingBoxMargin(10);
    _labelsGeode->addChild(_redScore);

    y -= 50;
    _vehiclePoseState = new HudTopLabel("", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_vehiclePoseState);

    y -= 60;
    _vehicleDrivetrainState = new HudTopLabel("", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile);
    _labelsGeode->addChild(_vehicleDrivetrainState);

    y = 100;
    _labelsGeode->addChild(new HudBottomLabel("[1] Top-down view", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile));
    if (numPlayers == 1)
    {
        y -= 20;
        _labelsGeode->addChild(new HudBottomLabel("[2] Orbit view", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile));
    }
    y -= 20;
    _labelsGeode->addChild(new HudBottomLabel("[F] Toggle full screen", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile));
    y -= 20;
    _labelsGeode->addChild(new HudBottomLabel("[S] Toggle stats", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile));
    y -= 20;
    _labelsGeode->addChild(new HudBottomLabel("[Space] Reset view", _padding, y, P_FONT_SIZE, config.sim.assets.fontFile));
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
        HudElement* label = dynamic_cast<HudElement*>(node);
        label->onWindowResize(width, height);
    }
}



void Hud::displayConnectionStatus(const std::vector<bool>& connected)
{
    for (int i=0; i<_connectedLabels.size(); i++)
    {
        HudLabel* label = _connectedLabels.at(i);
        HudBox* labelBoundingBox = _connectedBoxes.at(i);
        char tmp[255];
        sprintf(tmp, connected.at(i) ? "Player %d connected" : "Player %d disconnected", i+1);
        label->setText(tmp);
        labelBoundingBox->setColor(connected.at(i) ? Color::Green : Color::Red);
        osg::BoundingBox bb = label->getBoundingBox();
        labelBoundingBox->setSize(bb, 5);
    }
}



void Hud::displayConnectionStatus(bool connected, int playerId)
{
    HudLabel* label = _connectedLabels.at(0);
    HudBox* labelBoundingBox = _connectedBoxes.at(0);

    char tmp[255];
    sprintf(tmp, connected ? "Player %d connected" : "Player %d disconnected", playerId+1);
    label->setText(tmp);
    labelBoundingBox->setColor(connected ? Color::Green : Color::Red);

    osg::BoundingBox bb = label->getBoundingBox();
    labelBoundingBox->setSize(bb, 5);
}



void Hud::displayTimerStatus(bool running, double timerValue)
{
    if (timerValue == 0)
    {
        _timer->setColor(Color::Red);
    }
    else if (running && timerValue <= 10)
    {
        _timer->setColor(Color::Yellow);
    }
    else
    {
        _timer->setColor(Color::Black);
    }
    char tmp[1024];
    sprintf(tmp, "%d:%02d", (int) timerValue / 60, (int) std::ceil(timerValue) % 60);
    _timer->setText(tmp);
}



void Hud::displayFieldScore(int blueScore, int redScore)
{
    char tmp[1024];
    sprintf(tmp, "%d", blueScore);
    _blueScore->setText(tmp);
    sprintf(tmp, "%d", redScore);
    _redScore->setText(tmp);
}



void Hud::displayVehicleState(const SimState& state, int playerId)
{
    if (state.vehicles.size()-1 > playerId) { return; }
    SimState::VehicleState vehicleState = state.vehicles.at(playerId);

    char tmp[1024];

    double theta = vehicleState.theta*RADS_TO_DEG;
    while (theta < 0) { theta += 360; }
    while (theta > 360) { theta -= 360; }

    sprintf(tmp, "Pose: \n    X: %.1f ft \n    Y: %.1f ft \n    Theta: %.0f deg", vehicleState.x*M_TO_FT, vehicleState.y*M_TO_FT, theta);
    _vehiclePoseState->setText(tmp);

    sprintf(tmp, "Drivetrain: \n    Left motor: %.0f RPM \n    Right motor: %.0f RPM", vehicleState.leftDriveMotorSpeed*RADS_PER_SEC_TO_RPM, vehicleState.rightDriveMotorSpeed*RADS_PER_SEC_TO_RPM);
    _vehicleDrivetrainState->setText(tmp);
}
