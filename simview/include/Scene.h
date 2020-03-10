/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_SCENE_H
#define ROBOT_SIM_SCENE_H

#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "VehicleView.h"
#include "FieldView.h"
#include "GamePieceView.h"
#include "Types.h"


/**
 * Responsible for rendering the robot according to the incoming robot state.
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
    void update(const SimState& simState);

    /**
     * Returns the root node of the scene
     */
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

    /**
     * Returns a pointer to the vehicle node (for the Visualizer to center its view on the robot
     */
    osg::ref_ptr<osg::Node> getVehicleNode() const { return _vehicleViews.at(0)->getVehicleNode(); }

    /**
     * Returns the position of the vehicle in the scene
     */
    osg::Vec3d getVehiclePosition() const { return _vehicleViews.at(0)->getPosition(); }

private:
    const ConfigReader& _config;
    osg::ref_ptr<osg::Group> _root;
    std::vector<osg::ref_ptr<VehicleView>> _vehicleViews;
    osg::ref_ptr<FieldView> _fieldView;
    std::vector<osg::ref_ptr<GamePieceView>> _gamePieceViews;
};


#endif //ROBOT_SIM_SCENE_H
