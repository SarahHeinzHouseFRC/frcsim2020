/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_LABEL_H
#define ROBOT_SIM_LABEL_H

#include <osgText/Text>
#include "Color.h"


class HudElement
{
public:
    /**
     * HudLabel user is responsible for calling this method whenever the window is resized
     */
    virtual void onWindowResize(int windowWidth, int windowHeight) = 0;
};


/**
 * Utility class for labels usable in the Hud
 */
class HudLabel : public osgText::Text, public HudElement
{
public:
    /**
     * Constructor
     * @param text Text to display
     * @param pos Z-coordinate to display wrt top, bottom, or middle
     * @param fontSize Character height
     */
    HudLabel(const std::string& text, float x, float y, float fontSize, const std::string& fontFile);

protected:
    float _x;
    float _y;
};



class HudTopLabel : public HudLabel
{
public:
    /**
     * Constructor
     */
    HudTopLabel(const std::string& text, float x, float y, float fontSize, const std::string& fontFile) : HudLabel(text, x, y, fontSize, fontFile) {};

    /**
     * HudTopLabels will always position themselves relative to the top of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_x, _y + windowHeight, 0));
    }
};



class HudMiddleLabel : public HudLabel
{
public:
    /**
     * Constructor
     */
    HudMiddleLabel(const std::string& text, float x, float y, float fontSize, const std::string& fontFile) : HudLabel(text, x, y, fontSize, fontFile) {};

    /**
     * MiddleHudLabels will always position themselves relative to the vertical middle of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_x, _y + windowHeight/2, 0));
    }
};



class HudBottomLabel : public HudLabel
{
public:
    /**
     * Constructor
     */
    HudBottomLabel(const std::string& text, float x, float y, float fontSize, const std::string& fontFile) : HudLabel(text, x, y, fontSize, fontFile) {};

    /**
     * HudBottomLabels will always position themselves relative to the bottom of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_x, _y, 0));
    }
};



class HudBox : public osg::Geometry, public HudElement
{
public:
    /**
     * Constructor
     */
    HudBox(float x, float y, float width, float height, const Color& color);

    /**
     * Repositions itself according to x, y from the top left of the window
     * @param windowWidth
     * @param windowHeight
     */
    virtual void onWindowResize(int windowWidth, int windowHeight);

private:
    float _x, _y, _width, _height;
    osg::ref_ptr<osg::Vec3Array> _vertices;
    osg::ref_ptr<osg::Vec4Array> _colors;
};

#endif //ROBOT_SIM_LABEL_H
