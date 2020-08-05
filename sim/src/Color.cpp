/**
 * Copyright (c) 2020 Team 3260
 */

#include "Color.h"


Color Color::Blue(31, 119, 180);
Color Color::Orange(255, 127, 14);
Color Color::Yellow(255, 255, 14);
Color Color::Green(44, 160, 44);
Color Color::Red(214, 39, 40);
Color Color::Purple(148, 103, 189);
Color Color::Brown(140, 86, 75);
Color Color::Pink(227, 119, 194);
Color Color::Gray(127, 127, 127);
Color Color::DarkGray(40, 40, 40);
Color Color::Olive(188, 189, 34);
Color Color::Cyan(23, 190, 207);
Color Color::Black(0, 0, 0);
Color Color::White(255, 255, 255);

osg::Vec4 Color::jetmap(double gray)
{
    return osg::Vec4(jetmapHelper(gray - 0.25), jetmapHelper(gray), jetmapHelper(gray+0.25), 1);
}



osg::Vec4 Color::darken(osg::Vec4 color, double percent)
{
    double r = color.x() * (1.0-percent);
    double g = color.y() * (1.0-percent);
    double b = color.z() * (1.0-percent);
    double a = color.w();
    return osg::Vec4(r, g, b, a);
}



osg::Vec4 Color::lighten(osg::Vec4 color, double percent)
{
    double r = color.x() + (1.0-color.x()) * (percent);
    double g = color.y() + (1.0-color.y()) * (percent);
    double b = color.z() + (1.0-color.z()) * (percent);
    double a = color.w();
    return osg::Vec4(r, g, b, a);
}



double Color::jetmapHelper(double val)
{
    if      (val <= 0.125) { return 0; }
    else if (val <= 0.375) { return interpolate(val, 0.0, 0.125, 1.0, 0.375); }
    else if (val <= 0.625) { return 1.0; }
    else if (val <= 0.875) { return interpolate(val, 1.0, 0.625, 0.0, 0.875); }
    else return 0.0;
}
