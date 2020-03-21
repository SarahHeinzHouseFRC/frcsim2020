/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_GAMEPIECEVIEW_H
#define ROBOT_SIM_GAMEPIECEVIEW_H

#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "Types.h"


class GamePieceView : public osg::PositionAttitudeTransform
{
public:
    /**
     * Constructor
     */
    GamePieceView(const ConfigReader& config, int id);

    /**
     * Updates the view to reflect the model
     */
    void update(const SimState::GamePieceState& state);

private:
    /**
     * Draws the game piece as a yellow sphere
     */
    osg::ref_ptr<osg::Geode> drawGamePiece(const ConfigReader& config);

    osg::ref_ptr<osg::Node> _gamePieceNode;
    osg::ref_ptr<osg::ShapeDrawable> _shape;
    float _radius;
};


#endif //ROBOT_SIM_GAMEPIECEVIEW_H
