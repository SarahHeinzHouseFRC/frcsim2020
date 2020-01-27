/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "FieldModel.h"

using namespace Geometry;


FieldModel::FieldModel(const ConfigReader& config, double startTimestamp)
{
    std::vector<Vertex2d> exteriorVertices;
//    exteriorVertices.push_back({ 0, 10 });
//    exteriorVertices.push_back({ 0, -10 });

    exteriorVertices.push_back({ 4.11, 7.33 });
    exteriorVertices.push_back({ 2.31, 8.00 });
    exteriorVertices.push_back({ -2.31, 8.00 });
    exteriorVertices.push_back({ -4.11, 7.33 });
    exteriorVertices.push_back({ -4.11, -7.33 });
    exteriorVertices.push_back({ -2.31, -8.00 });
    exteriorVertices.push_back({ 2.31, -8.00 });
    exteriorVertices.push_back({ 4.11, -7.33 });
    _exteriorPolygon = Polygon2d(exteriorVertices);
}



void FieldModel::update(double currTimestamp)
{

}
