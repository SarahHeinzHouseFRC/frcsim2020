/**
 * Copyright (c) 2020 Team 3260
 */

#include "FieldModel.h"

using namespace Geometry;


FieldModel::FieldModel(const ConfigReader& config, double startTimestamp) :
        _inCollision(false),
        _collisionCount(0),
        _currTimestamp(startTimestamp),
        _timeLastCollision(startTimestamp),
        _blueScore(0),
        _redScore(0),
        _prevOuttakeButtonState(0),
        _exteriorWall(config.sim.field.exteriorWall),
        _rightTrenchRightWall(config.sim.field.rightTrenchRightWall),
        _rightTrenchLeftWall(config.sim.field.rightTrenchLeftWall),
        _leftTrenchRightWall(config.sim.field.leftTrenchRightWall),
        _leftTrenchLeftWall(config.sim.field.leftTrenchLeftWall),
        _rightColumn(config.sim.field.rightColumn),
        _topColumn(config.sim.field.topColumn),
        _leftColumn(config.sim.field.leftColumn),
        _bottomColumn(config.sim.field.bottomColumn),
        _blueGoalRegion(config.sim.field.blueGoalRegion),
        _redGoalRegion(config.sim.field.redGoalRegion),
        _blueOuttake(config.sim.field.blueOuttake),
        _redOuttake(config.sim.field.redOuttake)
{
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
