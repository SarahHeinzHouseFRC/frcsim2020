/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_FIELDVIEW_H
#define ROBOT_SIM_FIELDVIEW_H

#include <osg/PositionAttitudeTransform>
#include "ConfigReader.h"
#include "FieldModel.h"


class FieldView : public osg::PositionAttitudeTransform
{
public:
    /**
     * Constructor
     */
    FieldView(const ConfigReader& config);

    /**
     * Updates the field view based on the field model
     */
    void update(const FieldModel& fieldModel);

private:
    /**
     * Builds the visualization of the field
     */
    osg::ref_ptr<osg::Geode> makeField();

private:
    osg::ref_ptr<osg::Node> _fieldNode;
};


#endif //ROBOT_SIM_FIELDVIEW_H
