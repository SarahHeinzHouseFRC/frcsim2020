/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_FIELDMODEL_H
#define ROBOT_SIM_FIELDMODEL_H

#include "ConfigReader.h"


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
};


#endif //ROBOT_SIM_FIELDMODEL_H
