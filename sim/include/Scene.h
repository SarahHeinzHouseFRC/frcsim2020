/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "WorldModel.h"
#include "VehicleView.h"
#include "FieldView.h"
#include "GamePieceView.h"
#include "AbstractDrawer.h"


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
    void update(const SimState& simState, bool showLidar);

    /**
     * Returns the root node of the scene
     */
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

    /**
     * Returns a pointer to the vehicle node (for the Visualizer to center its view on the robot
     */
    osg::ref_ptr<osg::Node> getVehicleNode(int playerId) const { return _vehicleViews.at(playerId)->getVehicleNode(); }

    /**
     * Returns the position of the vehicle in the scene
     */
    osg::Vec3d getVehiclePosition(int playerId) const { return _vehicleViews.at(playerId)->getPosition(); }

    /**
     * Clears the list of drawers
     */
    void clearDrawers() { _drawers.clear(); }

    /**
     * Appends more drawers to the list
     */
    void addDrawers(const std::vector<std::shared_ptr<AbstractDrawer>> drawers) { _drawers.insert(_drawers.end(), drawers.begin(), drawers.end()); }

    /**
     * Sets whether or not to show the LIDAR rays
     */
    void showLidar(bool show) { _showLidar = show; }

private:
    osg::ref_ptr<osg::Group> _root;
    std::vector<osg::ref_ptr<VehicleView>> _vehicleViews;
    osg::ref_ptr<FieldView> _fieldView;
    std::vector<osg::ref_ptr<GamePieceView>> _gamePieceViews;
    bool _showLidar;
    std::vector<std::shared_ptr<AbstractDrawer>> _drawers;
    osg::ref_ptr<osg::Geode> _drawings;
};
