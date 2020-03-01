/**
 * Copyright (c) 2020 FRC Team 3260
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
    FieldView(const ConfigReader& config, const FieldModel& fieldModel);

    /**
     * Updates the field view based on the field model
     */
    void update(const FieldModel& fieldModel);

private:
    /**
     * Draws the ground plane as a gray rectangle
     */
    osg::ref_ptr<osg::Geode> drawGround(const FieldModel& fieldModel);

    /**
     * Draws the bounding collision polygon of the field as green walls
     */
    osg::ref_ptr<osg::Geode> drawBoundary(const FieldModel& fieldModel);

    /**
     * Draws the goal regions as red and blue outlined regions
     */
    osg::ref_ptr<osg::Geode> drawGoals(const FieldModel& fieldModel);

private:
    osg::ref_ptr<osg::Node> _fieldNode;
    osg::ref_ptr<osg::Geode> _fieldBounds;
};


#endif //ROBOT_SIM_FIELDVIEW_H
