/**
 * Copyright (c) 2020 Team 3260
 */

#include <osgGA/TerrainManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include "Color.h"
#include "Visualizer.h"


Visualizer::Visualizer(Scene &scene, Hud& hud, bool headless) : _scene(scene), _hud(hud), _windowWidth(1280), _windowHeight(720)
{
    if (headless) { return; }
    // Initialize key switch manipulator
    _keySwitchManipulator = new osgGA::KeySwitchMatrixManipulator;

    // Initialize terrain manipulator
    osg::ref_ptr<osgGA::TerrainManipulator> terrainManip = new osgGA::TerrainManipulator;
    terrainManip->setAllowThrow(false);
    _keySwitchManipulator->addMatrixManipulator('1', "Terrain", terrainManip);

    _viewer.setCameraManipulator(_keySwitchManipulator);
    _viewer.getCamera()->setClearColor(Color::Black);
    _viewer.setUpViewInWindow(0, 0, _windowWidth, _windowHeight);
    osgViewer::Viewer::Windows windows;
    _viewer.getWindows(windows);
    windows.at(0)->setWindowName("FRC 2020 Simulator");
    _viewer.setSceneData(_scene.getRoot());

    _viewer.addEventHandler(new osgViewer::StatsHandler);
    _viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    osg::Camera* camera = _hud.getCamera();
    camera->setViewport(0, 0, _windowWidth, _windowHeight);
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, _windowWidth, 0, _windowHeight));
    _viewer.addSlave(camera, false);
    _hud.onWindowResize(_windowWidth, _windowHeight);
    _scene.getRoot()->addChild(camera);
}



Visualizer::Visualizer(Scene &scene, Hud& hud, int playerId) : Visualizer(scene, hud)
{
    // Initialize node tracker manipulator
    osg::ref_ptr<osgGA::NodeTrackerManipulator> nodeTrackerManip = new osgGA::NodeTrackerManipulator;
    osg::Vec3d pos = _scene.getVehiclePosition(playerId);
    nodeTrackerManip->setHomePosition(osg::Vec3d(pos.x()-5, pos.y(), pos.z()+1),
                                      osg::Vec3d(pos.x(), pos.y(), pos.z()),
                                      osg::Vec3d(0, 0, 1));
    nodeTrackerManip->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_AZIM);
    nodeTrackerManip->setRotationMode(osgGA::NodeTrackerManipulator::ELEVATION_AZIM);
    nodeTrackerManip->setTrackNode(_scene.getVehicleNode(playerId));
    nodeTrackerManip->setAllowThrow(false);
    _keySwitchManipulator->addMatrixManipulator('2', "Trackball", nodeTrackerManip);
}



void Visualizer::step()
{
    _viewer.frame();

    const auto *traits = _viewer.getCamera()->getGraphicsContext()->getTraits();
    if (traits->width != _windowWidth || traits->height != _windowHeight)
    {
        _windowWidth = traits->width;
        _windowHeight = traits->height;
        osg::Camera* camera = _hud.getCamera();
        camera->setViewport(0, 0, _windowWidth, _windowHeight);
        camera->setProjectionMatrix(osg::Matrix::ortho2D(0, _windowWidth, 0, _windowHeight));
        _hud.onWindowResize(_windowWidth, _windowHeight);
    }
}
