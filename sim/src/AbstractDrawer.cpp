/**
 * Copyright (c) 2020 Team 3260
 */

#include "AbstractDrawer.h"
#include "ViewUtils.h"

std::map<std::string, Color> AbstractDrawer::_colors = {
    { "red", Color::Red },
    { "green", Color::Green },
    { "blue", Color::Blue },
    { "yellow", Color::Yellow }
};


AbstractDrawer::AbstractDrawer(const std::string& text, const std::string& color) : _text(text)
{
    auto it = _colors.find(color);
    _color = (it != _colors.end()) ? it->second : Color::Black;
}



BoxDrawer::BoxDrawer(const std::string& text, const std::string& color, float x, float y, float width, float height) :
        AbstractDrawer(text, color),
        _x(x),
        _y(y),
        _width(width),
        _height(height)
{
}



std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> BoxDrawer::draw()
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setCharacterSize(0.1);
    text->setPosition({ _x + _width/2, _y + _height/2, 0.1 });
    text->setText(_text);
    text->setColor(_color);
    text->setAutoRotateToScreen(true);
    text->setFont("/data/fonts/helvetica.ttf");

    return { ViewUtils::drawBox2d(_x, _y, 0.1, _width, _height, _color), text };
}



LineDrawer::LineDrawer(const std::string &text, const std::string &color, const std::vector<std::pair<float, float>>& vertices) :
        AbstractDrawer(text, color),
        _vertices(vertices)
{
}



std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> LineDrawer::draw()
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setCharacterSize(0.1);
    text->setPosition({ std::get<0>(_vertices.at(0)), std::get<1>(_vertices.at(0)), 0.1 });
    text->setText(_text);
    text->setColor(_color);
    text->setAutoRotateToScreen(true);
    text->setFont("/data/fonts/helvetica.ttf");

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    for (const auto& [x, y] : _vertices)
    {
        vertices->push_back({ x, y, 0.1 });
    }

    return { ViewUtils::drawLineStrip(vertices, _color), text };
}
