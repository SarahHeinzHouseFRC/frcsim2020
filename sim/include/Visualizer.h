/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once


#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include "Scene.h"
#include "Hud.h"


/**
 * Opens the window, visualizes, the scene, and defines how the user moves the camera around the scene
 */
class Visualizer
{
public:
    /**
     * Constructor
     */
    Visualizer(Scene& scene, Hud& hud, bool headless=false);

    /**
     * Constructor for a specific player
     */
    Visualizer(Scene& scene, Hud& hud, int playerId);

    /**
     * Steps the visualizer forward
     */
    void step();

    /**
     * Returns whether visualizer is done
     */
    bool done() const { return _viewer.done(); }

private:
    osgViewer::Viewer _viewer;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> _keySwitchManipulator;
    int _windowWidth;
    int _windowHeight;
    Scene& _scene;
    Hud& _hud;
};
