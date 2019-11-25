/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgGA/TerrainManipulator>
#include "Color.h"
#include "Visualizer.h"


Visualizer::Visualizer(const Scene &scene)
{
    osg::ref_ptr<osgGA::TerrainManipulator> manip = new osgGA::TerrainManipulator;
    manip->setAllowThrow(false);
    _viewer.setCameraManipulator(manip);
    _viewer.getCamera()->setClearColor(Color::Black);
    _viewer.setUpViewInWindow(0, 0, 1280, 720);
    osgViewer::Viewer::Windows windows;
    _viewer.getWindows(windows);
    windows.at(0)->setWindowName("Robot Simulator");
    _viewer.setSceneData(scene.getRoot());
}



void Visualizer::step()
{
    _viewer.frame();
}
