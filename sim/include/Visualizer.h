/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_VISUALIZER_H
#define ROBOT_SIM_VISUALIZER_H


#include <osgViewer/Viewer>
#include "Scene.h"


/**
 * Opens the window, visualizes, the scene, and defines how the user moves the camera around the scene
 */
class Visualizer
{
public:
    /**
     * Constructor
     */
    Visualizer(const Scene& scene);

    /**
     * Steps the visualizer forward
     */
    void step();

    /**
     * Returns whether visualizer is done
     */
    bool done() { return _viewer.done(); }

private:
    osgViewer::Viewer _viewer;
};


#endif //ROBOT_SIM_VISUALIZER_H
