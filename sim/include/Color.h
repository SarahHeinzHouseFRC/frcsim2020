/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

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
    static Color DarkGray;
    static Color Olive;
    static Color Cyan;
    static Color Black;
    static Color White;

    /**
     * Method for converting a grayscale value to a jetmap
     * @param gray Grayscale value 0-1
     * @return Jetmapped RGB values 0-1
     */
    static osg::Vec4 jetmap(double gray);

    /**
     * Darkens the given color by the given percent (0-1). Alpha value is
     * unchanged.
     */
    static osg::Vec4 darken(osg::Vec4 color, double percent);

    /**
     * Lightens the given color by the given percent (0-1). Alpha value is
     * unchanged.
     */
    static osg::Vec4 lighten(osg::Vec4 color, double percent);

    /**
     * Shades the given color by the given percent (0-1). If the percent is <0.5,
     * the color is lightened and darkened otherwise. Alpha value is unchanged.
     */
    inline static osg::Vec4 shade(osg::Vec4 color, double percent)
    {
        return (percent < 0.5) ? darken(color, 2*(0.5-percent)) : lighten(color, 2*(percent-0.5));
    }

private:
    /**
     * Helper method for jetmap.
     * Source: https://stackoverflow.com/questions/7706339/grayscale-to-red-green-blue-matlab-jet-color-scale
     * @param val Grayscale value 0-1
     * @return Jet-mapped RGB values 0-1. Low values are blue, medium values are green, high values are red
     */
    static double jetmapHelper(double val);

    /**
     * Helper method for jetmapHelper
     */
    inline static double interpolate(double val, double y0, double x0, double y1, double x1)
    {
        return (val-x0)*(y1-y0)/(x1-x0) + y0;
    }
};
