/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_HUD_H
#define ROBOT_SIM_HUD_H


#include <vector>
#include <osg/Camera>
#include <osgText/Text>
#include "ConfigReader.h"


class Hud
{
public:
    /**
     * Constructor
     */
    Hud(const ConfigReader& config);

    /**
     * Returns the HUD's camera
     */
    osg::Camera* getCamera() { return _camera; }

    /**
     * Callback called by the visualizer
     * @param width New window width (pixels)
     * @param height New window height (pixels)
     */
    void onWindowResize(int width, int height);

private:
    class Label : public osgText::Text
    {
    public:
        enum Alignment
        {
            FROM_TOP, FROM_MIDDLE, FROM_BOTTOM
        };

        Label(const std::string& text, Alignment a, const osg::Vec3& pos, float fontSize=16);

        void updatePosition(int windowWidth, int windowHeight);

    private:
        osg::Vec3 _desiredPos;
        Alignment _alignment;
    };

    void addLabel(osg::ref_ptr<Label> label);

    osg::Camera* _camera;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::Geode> _geode;
    osg::ref_ptr<osg::Geometry> _geom;
    osg::ref_ptr<osg::Vec3Array> _vertices;
    int _width;
    std::vector<osg::ref_ptr<Label>> _labels;
    static constexpr float _leftPadding = 15;
};


#endif //ROBOT_SIM_HUD_H
