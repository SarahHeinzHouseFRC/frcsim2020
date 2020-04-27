/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_FIELDMODEL_H
#define ROBOT_SIM_FIELDMODEL_H

#include <tuple>
#include "ConfigReader.h"
#include "Geometry.h"
#include "BaseModel.h"
#include "Types.h"


class FieldModel : public BaseModel
{
friend class FieldView;
friend class PhysicsEngine;
friend class WorldModel;
public:
    /**
     * Constructor
     */
    FieldModel(const ConfigReader& config, double startTimestamp);

    /**
     * Updates the field to reflect the current time
     */
    void update(double currTimestamp);

    /**
     * Returns the model type
     */
    virtual ModelType modelType() { return FIELD_MODEL; }

    /**
     * Collision callback
     */
    virtual void isInCollision(bool c);

    /**
     * Returns the collision count
     */
    int getCollisionCount() { return _collisionCount; }

    /**
     * Returns the current score
     */
    std::tuple<int, int> getScore() { return { _blueScore, _redScore }; }

    /**
     * Resets the collision count
     */
    void reset() { _collisionCount = 0; _inCollision = false; }

private:
    int _prevOuttakeButtonState;
    double _currTimestamp;
    double _timeLastCollision;
    bool _inCollision;
    int _collisionCount;
    Geometry::Polygon2d _exteriorWall;
    Geometry::Polygon2d _rightTrenchRightWall;
    Geometry::Polygon2d _rightTrenchLeftWall;
    Geometry::Polygon2d _leftTrenchRightWall;
    Geometry::Polygon2d _leftTrenchLeftWall;
    Geometry::Polygon2d _rightColumn;
    Geometry::Polygon2d _topColumn;
    Geometry::Polygon2d _leftColumn;
    Geometry::Polygon2d _bottomColumn;
    Geometry::Polygon2d _blueGoalRegion;
    Geometry::Polygon2d _redGoalRegion;
    Geometry::Vertex2d _blueOuttake;
    Geometry::Vertex2d _redOuttake;
    int _blueScore;
    int _redScore;
};


#endif //ROBOT_SIM_FIELDMODEL_H
