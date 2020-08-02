/**
 * Copyright (c) 2020 Team 3260
 */
#pragma once

#include <osgGA/GUIEventHandler>


class KeyHandler : public osgGA::GUIEventHandler
{
public:
    /**
     * Constructor
     */
    KeyHandler();

    /**
     * Handles keyboard events
     */
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);

    /**
     * Whether or not to show LIDAR
     */
    bool showLidar() { return _showLidar; }

    /**
     * Whether or not to reset the world
     */
    bool reset() { return _reset; }

    /**
     * Resets all flags to initial values
     */
    void resetFlags() { _reset = false; }

private:
    bool _showLidar;
    bool _reset;
};
