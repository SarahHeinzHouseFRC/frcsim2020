/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_VEHICLEVIEW_H
#define ROBOT_SIM_VEHICLEVIEW_H

#include <osg/PositionAttitudeTransform>
#include "VehicleModel.h"
#include "ConfigReader.h"


class VehicleView : public osg::PositionAttitudeTransform
{
public:
    /**
     * Constructor
     */
    VehicleView(const ConfigReader& config, const VehicleModel& vehicleModel);

    /**
     * Updates the vehicle view based on the vehicle model
     */
    void update(const VehicleModel& vehicleModel);

    /**
     * Returns vehicle node (for Visualizer to center view on)
     */
    osg::ref_ptr<osg::Node> getVehicleNode() { return _vehicleNode; }

private:
    /**
     * Helper method for creating a default robot out of basic geometry
     */

    osg::ref_ptr<osg::Geode> drawVehicle(const ConfigReader& config);

    /**
     * Draws the collision boundary of the vehicle
     */
    osg::ref_ptr<osg::Geode> drawCollisionBoundary(const VehicleModel& vehicleModel);

    /**
     * Draws the ingestible region of the vehicle
     */
    osg::ref_ptr<osg::Geode> drawIngestibleRegions(const VehicleModel& vehicleModel);

    osg::ref_ptr<osg::Node> _vehicleNode;
    osg::ref_ptr<osg::Geode> _vehicleBounds;
    float _wheelRadius;
};


#endif //ROBOT_SIM_VEHICLEVIEW_H
