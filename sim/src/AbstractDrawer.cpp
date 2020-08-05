/**
 * Copyright (c) 2020 Team 3260
 */

#include "AbstractDrawer.h"
#include "ViewUtils.h"

std::map<std::string, Color> AbstractDrawer::_colors = {
    { "red", Color::Red },
    { "green", Color::Green },
    { "blue", Color::Blue },
    { "yellow", Color::Yellow },
    { "darkgray", Color::DarkGray }
};

float z = 0.01;


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
    text->setPosition({ _x + _width/2, _y + _height/2, z });
    text->setText(_text);
    text->setColor(_color);
    text->setAutoRotateToScreen(true);
    text->setFont("/data/fonts/helvetica.ttf");

    return { ViewUtils::drawBox2d(_x, _y, z, _width, _height, _color), text };
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
    text->setPosition({ std::get<0>(_vertices.at(0)), std::get<1>(_vertices.at(0)), z });
    text->setText(_text);
    text->setColor(_color);
    text->setAutoRotateToScreen(true);
    text->setFont("/data/fonts/helvetica.ttf");

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    for (const auto& [x, y] : _vertices)
    {
        vertices->push_back({ x, y, z });
    }

    return { ViewUtils::drawLineStrip(vertices, _color), text };
}



GridDrawer::GridDrawer(const std::string& text, const std::string& color, int numCols, int numRows, float cellSize, const std::vector<uint8_t>& occupancy) :
        AbstractDrawer(text, color),
        _numCols(numCols),
        _numRows(numRows),
        _cellSize(cellSize),
        _occupancy(occupancy)
{
}



std::tuple<osg::ref_ptr<osg::Drawable>, osg::ref_ptr<osg::Drawable>> GridDrawer::draw()
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setCharacterSize(0.1);
    text->setPosition({0, 0, z});
    text->setText(_text);
    text->setColor(_color);
    text->setAutoRotateToScreen(true);
    text->setFont("/data/fonts/helvetica.ttf");

    osg::ref_ptr<osg::Geometry> grid = ViewUtils::drawGrid(_numCols, _numRows, _cellSize, z, _color);

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    for (int i=0; i<_occupancy.size(); i++)
    {
        float p = 255 * _occupancy.at(i) / 100.0;
        if (p > 0)
        {
            int col = i / _numRows;
            int row = i % _numRows;
            float x = (col - _numCols/2.0) * _cellSize + _cellSize/2;
            float y = (row - _numRows/2.0) * _cellSize + _cellSize/2;
            vertices->push_back({ x - _cellSize/2, y - _cellSize/2, z });
            vertices->push_back({ x + _cellSize/2, y - _cellSize/2, z });
            vertices->push_back({ x + _cellSize/2, y + _cellSize/2, z });
            vertices->push_back({ x - _cellSize/2, y + _cellSize/2, z });
            colors->push_back(Color(_color, p));
            colors->push_back(Color(_color, p));
            colors->push_back(Color(_color, p));
            colors->push_back(Color(_color, p));
        }
    }
    osg::ref_ptr<osg::Geometry> occupied = ViewUtils::drawQuads(vertices, colors);

    return { grid, occupied };
}
