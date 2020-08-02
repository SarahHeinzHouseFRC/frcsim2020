/**
 * Copyright (c) 2020 Team 3260
 */

#include "KeyHandler.h"


KeyHandler::KeyHandler() : _showLidar(true), _reset(false)
{

}



bool KeyHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
    {
        switch (ea.getKey())
        {
            case osgGA::GUIEventAdapter::KEY_L:
                _showLidar = !_showLidar;
                return true;

            case osgGA::GUIEventAdapter::KEY_R:
                _reset = true;
                return true;

            default:
                return false;
        }
    }
    return false;
}
