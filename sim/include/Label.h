/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_LABEL_H
#define ROBOT_SIM_LABEL_H

#include <osgText/Text>


/**
 * Utility class for labels usable in the Hud
 */
class Label : public osgText::Text
{
public:
    /**
     * Constructor
     * @param text Text to display
     * @param pos Z-coordinate to display wrt top, bottom, or middle
     * @param fontSize Character height
     */
    Label(const std::string& text, float pos, float fontSize=16);

    /**
     * Label user is responsible for calling this method whenever the window is resized
     */
    virtual void onWindowResize(int windowWidth, int windowHeight) = 0;

protected:
    float _desiredPos;
    static constexpr float _leftPadding = 15;
};



class TopLabel : public Label
{
public:
    /**
     * Constructor
     */
    TopLabel(const std::string& text, float pos, float fontSize=16) : Label(text, pos, fontSize) {};

    /**
     * TopLabels will always position themselves relative to the top of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_leftPadding, _desiredPos + windowHeight, 0));
    }
};



class MiddleLabel : public Label
{
public:
    /**
     * Constructor
     */
    MiddleLabel(const std::string& text, float pos, float fontSize=16) : Label(text, pos, fontSize) {};

    /**
     * MiddleLabels will always position themselves relative to the vertical middle of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_leftPadding, _desiredPos + windowHeight/2, 0));
    }
};



class BottomLabel : public Label
{
public:
    /**
     * Constructor
     */
    BottomLabel(const std::string& text, float pos, float fontSize=16) : Label(text, pos, fontSize) {};

    /**
     * BottomLabels will always position themselves relative to the bottom of the window
     */
    virtual void onWindowResize(int windowWidth, int windowHeight)
    {
        setPosition(osg::Vec3(_leftPadding, _desiredPos, 0));
    }
};


#endif //ROBOT_SIM_LABEL_H
