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

private:
    bool _showLidar;
};
