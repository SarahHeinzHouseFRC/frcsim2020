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
    Geometry::Polygon2d _exteriorPolygon;
    std::vector<Geometry::Polygon2d> _interiorPolygons;
    Geometry::Polygon2d _blueGoal;
    Geometry::Polygon2d _redGoal;
    Geometry::Vertex2d _blueOuttake;
    Geometry::Vertex2d _redOuttake;
    int _blueScore;
    int _redScore;
};


#endif //ROBOT_SIM_FIELDMODEL_H
