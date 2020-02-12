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
    osg::ref_ptr<osg::Geode> drawIngestibleRegion(const VehicleModel& vehicleModel);

    /**
     * Helper method to create a robot carriage
     */
    osg::ref_ptr<osg::PositionAttitudeTransform> drawElevator();

    osg::ref_ptr<osg::Node> _vehicleNode;
    osg::ref_ptr<osg::Geode> _vehicleBounds;
    osg::ref_ptr<osg::PositionAttitudeTransform> _elevatorPat;
    float _wheelRadius;
    float _beltRadius;
    float _beltWidth;
    float _beltLength;
    float _motorShaftRadius;
    float _motorShaftLength;
    float _motorRadius;
    float _motorLength;
    float _encoderRadius;
    float _encoderLength;
    float _carriageLengthX;
    float _carriageLengthY;
    float _carriageLengthZ;
};


#endif //ROBOT_SIM_VEHICLEVIEW_H
