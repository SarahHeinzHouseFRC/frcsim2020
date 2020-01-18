/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "VehicleView.h"
#include "VehicleModel.h"
#include "FieldView.h"
#include "FieldModel.h"


/**
 * Responsible for rendering the robot according to the robot model.
 */
class Scene
{
public:
    /**
     * Constructs the scene
     */
    Scene(const ConfigReader& config);

    /**
     * Updates the scene given the robot's current state
     */
    void update(const VehicleModel& vehicleModel, const FieldModel& fieldModel);

    /**
     * Returns the root node of the scene
     */
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

    /**
     * Returns a pointer to the vehicle node (for the Visualizer to center its view on the robot
     */
    osg::ref_ptr<osg::Node> getVehicleNode() const { return _vehicleView->getVehicleNode(); }

    /**
     * Returns the position of the vehicle in the scene
     */
    osg::Vec3d getVehiclePosition() const { return _vehicleView->getPosition(); }

private:
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<VehicleView> _vehicleView;
    osg::ref_ptr<FieldView> _fieldView;
};


#endif //ROBOT_SIM_SCENE_H
