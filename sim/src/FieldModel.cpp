/**
 * Copyright (c) 2020 Team 3260
 */

#include <ConfigReader.h>
#include "FieldModel.h"

using namespace Geometry;


FieldModel::FieldModel(const ConfigReader& config, double startTimestamp) :
        _inCollision(false), _collisionCount(0), _currTimestamp(startTimestamp), _timeLastCollision(startTimestamp)
{
    // Exterior polygon
    _exteriorPolygon = Polygon2d(config.sim.field.exteriorPolygon);

    // Interior polygons
    for (const auto& interiorPolygon : config.sim.field.interiorPolygons)
    {
        _interiorPolygons.emplace_back(interiorPolygon);
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
