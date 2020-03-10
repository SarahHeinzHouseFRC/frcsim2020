/**
 * Copyright (c) 2020 Team 3260
 */

#include "FieldModel.h"

#define IN_TO_M 0.0254f

using namespace Geometry;


FieldModel::FieldModel(const ConfigReader& config, double startTimestamp) :
        _inCollision(false),
        _collisionCount(0),
        _currTimestamp(startTimestamp),
        _timeLastCollision(startTimestamp),
        _blueScore(0),
        _redScore(0),
        _prevOuttakeButtonState(0),
        _blueOuttake(-61.00*0.0254, 314.96*0.0254),
        _redOuttake(61.00*0.0254, -314.96*0.0254)
{
    // Exterior polygon
    _exteriorPolygon = Polygon2d(config.sim.field.exteriorPolygon);

    // Interior polygons
    for (const auto& interiorPolygon : config.sim.field.interiorPolygons)
    {
        _interiorPolygons.emplace_back(interiorPolygon);
    }

    _blueGoal = std::vector<Vertex2d>({ { -84*IN_TO_M, -314.96*IN_TO_M }, { -84*IN_TO_M, -354.96*IN_TO_M }, { -50*IN_TO_M, -354.96*IN_TO_M }, { -50*IN_TO_M, -314.96*IN_TO_M } });
    _redGoal = std::vector<Vertex2d>({ { 84*IN_TO_M, 314.96*IN_TO_M }, { 84*IN_TO_M, 354.96*IN_TO_M }, { 50*IN_TO_M, 354.96*IN_TO_M }, { 50*IN_TO_M, 314.96*IN_TO_M } });
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
