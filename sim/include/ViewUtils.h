/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <osg/ShapeDrawable>
#include <osg/Geometry>


class ViewUtils
{
public:
    /**
     * Utility method for quickly drawing arrows
     */
    static osg::ref_ptr<osg::Geometry> drawArrow(const osg::Vec3d& center, float theta, float tailLength, const osg::Vec4& color);

    /**
     * Utility method for drawing points
     */
    static osg::ref_ptr<osg::Geometry> drawPoints(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing lines. Pairs of vertices are interpreted as single lines
     */
    static osg::ref_ptr<osg::Geometry> drawLines(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a line strip. Each vertex is connected to the previous by a line.
     */
    static osg::ref_ptr<osg::Geometry> drawLineStrip(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a line loop. Same as line strip but first and last vertices are also connected
     * with a line.
     */
    static osg::ref_ptr<osg::Geometry> drawLineLoop(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a cylinder
     */
    static osg::ref_ptr<osg::ShapeDrawable> drawCylinder(const osg::Vec3& pos, float radius, float height, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a box
     */
    static osg::ref_ptr<osg::ShapeDrawable> drawBox(const osg::Vec3& pos, float lengthX, float lengthY, float lengthZ, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a 2D box
     */
    static osg::ref_ptr<osg::Drawable> drawBox2d(float x, float y, float z, float lengthX, float lengthY, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing quads
     */
    static osg::ref_ptr<osg::Geometry> drawQuads(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing quads
     */
    static osg::ref_ptr<osg::Geometry> drawTriangleFan(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color);

    /**
     * Utility method for quickly drawing a grid
     */
    static osg::ref_ptr<osg::Geometry> drawGrid(int numCols, int numRows, float cellSize, float z, const osg::Vec4& color);

private:
    /**
     * Common helper method for quickly drawing geometries
     */
    static osg::ref_ptr<osg::Geometry> drawGeometry(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color, GLenum primitive);
};
