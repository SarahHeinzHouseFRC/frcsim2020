/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "CollisionDetector.h"

using namespace Geometry;


CollisionDetector::CollisionDetector()
{

}



void CollisionDetector::detectCollisions(FieldModel& fieldModel, VehicleModel& vehicleModel, GamePieceModel& gamePieceModel)
{
    bool c = collision(fieldModel.exteriorPolygon(), vehicleModel.polygon());
    fieldModel.collisionCallback(c);
    vehicleModel.collisionCallback(c);
}
