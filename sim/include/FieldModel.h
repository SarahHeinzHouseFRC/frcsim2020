/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_FIELDMODEL_H
#define ROBOT_SIM_FIELDMODEL_H

#include "ConfigReader.h"
#include "Geometry.h"


class FieldModel
{
friend class FieldView;
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
     * Returns exterior polygon
     */
    Geometry::Polygon2d exteriorPolygon() const { return _exteriorPolygon; }

    /**
     * Returns interior polygons
     */
    std::vector<Geometry::Polygon2d> interiorPolygons() const { return _interiorPolygons; }

private:
    Geometry::Polygon2d _exteriorPolygon;
    std::vector<Geometry::Polygon2d> _interiorPolygons;
};


#endif //ROBOT_SIM_FIELDMODEL_H
