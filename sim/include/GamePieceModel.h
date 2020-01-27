/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_GAMEPIECEMODEL_H
#define ROBOT_SIM_GAMEPIECEMODEL_H

#include "ConfigReader.h"


class GamePieceModel
{
friend class GamePieceView;
friend class CollisionDetector;
public:
    /**
     * Constructor
     */
    GamePieceModel(const ConfigReader& configReader) :
            _radius(configReader.sim.constants.gamePiece.radius), _state({ { 0, 0, 0, 0 } }) {};

    struct
    {
        struct
        {
            double x, y;
            double vx, vy;
        } pose;
    } _state;

private:
    double _radius;
};


#endif //ROBOT_SIM_GAMEPIECEMODEL_H
