/**
 * Copyright (c) 2020 Team 3260
 */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <osg/Drawable>
#include <osgText/Text>
#include "Color.h"


class AbstractDrawer
{
public:
    /**
     * Constructor
     */
    AbstractDrawer(const std::string& text, const std::string& color);

    /**
     * How to draw this shape
     */
    virtual std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> draw() = 0;

protected:
    static std::map<std::string, Color> _colors;
    std::string _text;
    Color _color;
};



class BoxDrawer : public AbstractDrawer
{
public:
    /**
     * Constructor
     */
    BoxDrawer(const std::string& text, const std::string& color, float x, float y, float width, float height);

    /**
     * Draw
     */
    virtual std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> draw();

private:
    float _x;
    float _y;
    float _width;
    float _height;
};



class LineDrawer : public AbstractDrawer
{
public:
    /**
     * Constructor
     */
    LineDrawer(const std::string& text, const std::string& color, const std::vector<std::pair<float, float>>& vertices);

    /**
     * Draw
     */
    virtual std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> draw();

public:
    std::vector<std::pair<float, float>> _vertices;
};
