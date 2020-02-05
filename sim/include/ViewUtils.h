/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_VIEWUTILS_H
#define ROBOT_SIM_VIEWUTILS_H

#include <osg/ShapeDrawable>
#include <osg/Geometry>


class ViewUtils
{
public:
    /**
     * Utility method for quickly drawing lines. Pairs of vertices are interpreted as single lines
     */
    static osg::ref_ptr<osg::Geometry> makeLines(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing lines. Pairs of vertices are interpreted as single lines
     */
    static osg::ref_ptr<osg::Geometry> makeLineLoop(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a cylinder
     */
    static osg::ref_ptr<osg::ShapeDrawable> makeCylinder(const osg::Vec3& pos, float radius, float height, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a box
     */
    static osg::ref_ptr<osg::ShapeDrawable> makeBox(const osg::Vec3& pos, float lengthX, float lengthY, float lengthZ, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing quads
     */
    static osg::ref_ptr<osg::Geometry> makeQuads(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing quads
     */
    static osg::ref_ptr<osg::Geometry> makeTriangleFan(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

private:
    /**
     * Common helper method for quickly drawing geometries
     */
    static osg::ref_ptr<osg::Geometry> makeBaseGeometry(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color, GLenum primitive);
};


#endif //ROBOT_SIM_VIEWUTILS_H
