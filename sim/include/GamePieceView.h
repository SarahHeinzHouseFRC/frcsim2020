/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_GAMEPIECEVIEW_H
#define ROBOT_SIM_GAMEPIECEVIEW_H

#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "GamePieceModel.h"


class GamePieceView : public osg::PositionAttitudeTransform
{
public:
    /**
     * Constructor
     */
    GamePieceView(const ConfigReader& config, const GamePieceModel& gamePieceModel);

    /**
     * Updates the view to reflect the model
     */
    void update(const GamePieceModel& gamePieceModel);

private:
    /**
     * Builds a visualization of the game piece
     */
    osg::ref_ptr<osg::Geode> makeView(const GamePieceModel& gamePieceModel);

    osg::ref_ptr<osg::Node> _node;
    osg::ref_ptr<osg::Geode> _boundary;
};


#endif //ROBOT_SIM_GAMEPIECEVIEW_H
