/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_COLOR_H
#define ROBOT_SIM_COLOR_H

#include <osg/Vec4>


/**
 * Utility class for quickly defining some default colors.
 */
class Color : public osg::Vec4
{
public:
    Color() = default;

    /**
     * Creates a color using the 0-255 scale out of red/green/blue(/alpha)
     */
    Color(float r, float g, float b, float a=255.0) : osg::Vec4(r/255.0, g/255.0, b/255.0, a/255.0) {};

    /**
     * Copy constructor with an option to modify alpha value
     */
    Color(const Color& c, float a=255.0) : osg::Vec4(c.r(), c.g(), c.b(), a/255.0) {};

    static Color Blue;
    static Color Orange;
    static Color Yellow;
    static Color Green;
    static Color Red;
    static Color Purple;
    static Color Brown;
    static Color Pink;
    static Color Gray;
    static Color Olive;
    static Color Cyan;
    static Color Black;
    static Color White;
};


#endif //ROBOT_SIM_COLOR_H
