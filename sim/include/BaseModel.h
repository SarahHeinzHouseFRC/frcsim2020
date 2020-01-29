/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_BASEMODEL_H
#define ROBOT_SIM_BASEMODEL_H


class BaseModel
{
public:
    enum ModelType
    {
        NONE_MODEL = 0,
        FIELD_MODEL = 1,
        VEHICLE_MODEL = 2,
        GAME_PIECE_MODEL = 4
    };

    virtual ModelType modelType() = 0;

    virtual void hasCollision(bool c) = 0;
};


#endif //ROBOT_SIM_BASEMODEL_H
