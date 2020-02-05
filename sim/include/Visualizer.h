/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_VISUALIZER_H
#define ROBOT_SIM_VISUALIZER_H


#include <osgViewer/Viewer>
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
    Visualizer(Scene& scene, Hud& hud);

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
    int _windowWidth;
    int _windowHeight;
    Scene& _scene;
    Hud& _hud;
};


#endif //ROBOT_SIM_VISUALIZER_H
