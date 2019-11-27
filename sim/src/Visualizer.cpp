/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgGA/TerrainManipulator>
#include "Color.h"
#include "Visualizer.h"


Visualizer::Visualizer(Scene &scene, Hud& hud) : _scene(scene), _hud(hud), _windowWidth(1280), _windowHeight(720)
{
    osg::ref_ptr<osgGA::TerrainManipulator> manip = new osgGA::TerrainManipulator;
    manip->setAllowThrow(false);
    _viewer.setCameraManipulator(manip);
    _viewer.getCamera()->setClearColor(Color::Black);
    _viewer.setUpViewInWindow(0, 0, _windowWidth, _windowHeight);
    osgViewer::Viewer::Windows windows;
    _viewer.getWindows(windows);
    windows.at(0)->setWindowName("Robot Simulator");
    _viewer.setSceneData(scene.getRoot());

    osg::Camera* camera = _hud.getCamera();
    camera->setViewport(0, 0, _windowWidth, _windowHeight);
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, _windowWidth, 0, _windowHeight));
    _viewer.addSlave(camera, false);
    _hud.onWindowResize(_windowWidth, _windowHeight);
    _scene.getRoot()->addChild(camera);
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
