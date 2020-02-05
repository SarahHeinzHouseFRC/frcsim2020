/**
 * Copyright (c) 2020 Team 3260
 */

#include "FieldModel.h"

using namespace Geometry;


FieldModel::FieldModel(const ConfigReader& config, double startTimestamp) :
        _inCollision(false), _collisionCount(0), _currTimestamp(startTimestamp), _timeLastCollision(startTimestamp)
{
    std::vector<Vertex2d> exteriorVertices;

    exteriorVertices.push_back({ 4.11, 7.33 });
    exteriorVertices.push_back({ 2.31, 8.00 });
    exteriorVertices.push_back({ -2.31, 8.00 });
    exteriorVertices.push_back({ -4.11, 7.33 });
    exteriorVertices.push_back({ -4.11, -7.33 });
    exteriorVertices.push_back({ -2.31, -8.00 });
    exteriorVertices.push_back({ 2.31, -8.00 });
    exteriorVertices.push_back({ 4.11, -7.33 });
    _exteriorPolygon = Polygon2d(exteriorVertices);

    {
        // Right trench right wall
        std::vector<Vertex2d> interiorVertices1;
        interiorVertices1.push_back({ 4.11, -1.52 });
        interiorVertices1.push_back({ 4.11, -0.76 });
        interiorVertices1.push_back({ 4.06, -0.76 });
        interiorVertices1.push_back({ 4.06, -1.52 });
        _interiorPolygons.emplace_back(interiorVertices1);
    }
    {
        // Right trench left wall
        std::vector<Vertex2d> interiorVertices1;
        interiorVertices1.push_back({ 2.73, -1.52 });
        interiorVertices1.push_back({ 2.73, -0.76 });
        interiorVertices1.push_back({ 2.68, -0.76 });
        interiorVertices1.push_back({ 2.68, -1.52 });
        _interiorPolygons.emplace_back(interiorVertices1);
    }
    {
        // Left trench right wall
        std::vector<Vertex2d> interiorVertices1;
        interiorVertices1.push_back({ -4.06, 0.76 });
        interiorVertices1.push_back({ -4.06, 1.52 });
        interiorVertices1.push_back({ -4.11, 1.52 });
        interiorVertices1.push_back({ -4.11, 0.76 });
        _interiorPolygons.emplace_back(interiorVertices1);
    }
    {
        // Left trench left wall
        std::vector<Vertex2d> interiorVertices1;
        interiorVertices1.push_back({ -2.68, 0.76 });
        interiorVertices1.push_back({ -2.68, 1.52 });
        interiorVertices1.push_back({ -2.73, 1.52 });
        interiorVertices1.push_back({ -2.73, 0.76 });
        _interiorPolygons.emplace_back(interiorVertices1);
    }
    {
        // Right column
        std::vector<Vertex2d> interiorVertices;
        interiorVertices.push_back({ 2.67, -1.21 });
        interiorVertices.push_back({ 2.55, -0.93 });
        interiorVertices.push_back({ 2.27, -1.05 });
        interiorVertices.push_back({ 2.39, -1.33 });
        _interiorPolygons.emplace_back(interiorVertices);
    }
    {
        // Top column
        std::vector<Vertex2d> interiorVertices;
        interiorVertices.push_back({ 1.14, 2.48 });
        interiorVertices.push_back({ 1.03, 2.75 });
        interiorVertices.push_back({ 0.75, 2.63 });
        interiorVertices.push_back({ 0.86, 2.35 });
        _interiorPolygons.emplace_back(interiorVertices);
    }
    {
        // Left column
        std::vector<Vertex2d> interiorVertices;
        interiorVertices.push_back({ -2.27, 1.05 });
        interiorVertices.push_back({ -2.39, 1.33 });
        interiorVertices.push_back({ -2.67, 1.21 });
        interiorVertices.push_back({ -2.55, 0.93 });
        _interiorPolygons.emplace_back(interiorVertices);
    }
    {
        // Bottom column
        std::vector<Vertex2d> interiorVertices;
        interiorVertices.push_back({ -0.75, -2.63 });
        interiorVertices.push_back({ -0.86, -2.35 });
        interiorVertices.push_back({ -1.14, -2.46 });
        interiorVertices.push_back({ -1.03, -2.75 });
        _interiorPolygons.emplace_back(interiorVertices);
    }
}



void FieldModel::update(double currTimestamp)
{
    _currTimestamp = currTimestamp;
}



void FieldModel::isInCollision(bool c)
{
    _inCollision = c;

    // Only count collisions after 1s
    if (_currTimestamp - _timeLastCollision > 1.0)
    {
        _collisionCount++;
        _timeLastCollision = _currTimestamp;
    }
}
