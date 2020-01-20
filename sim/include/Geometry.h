/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_GEOMETRY_H
#define ROBOT_SIM_GEOMETRY_H

#include <vector>
#include <cmath>


namespace Geometry
{

struct Vertex2d
{
    double x, y;

    /**
     * Default constructor
     */
    Vertex2d() = default;

    /**
     * Constructor
     */
    Vertex2d(double x, double y) : x(x), y(y) {};

    Vertex2d operator+(const Vertex2d& v) const
    {
        return { x + v.x , y + v.y };
    }

    Vertex2d operator-(const Vertex2d& v) const
    {
        return { x - v.x , y - v.y };
    }
};

/**
 * Whether segments AB and BC form a clockwise, counterclockwise, or no turn
 * @return 0 if a,b,c are collinear, 1 for clockwise, 2 for ccw
 */
int orientation(const Vertex2d& a, const Vertex2d& b, const Vertex2d& c);

/**
 * Given three collinear points, determines whether point B lies on segment AC
 */
bool onSegment(const Vertex2d& a, const Vertex2d& b, const Vertex2d& c);



struct Edge2d
{
    Vertex2d a;
    Vertex2d b;
};

/**
 * Checks for collisions between two segments
 * @return True if intersection, false for clear
 */
bool collision(const Edge2d& edge1, const Edge2d& edge2);



class Polygon2d
{
public:
    /**
     * Default constructor
     */
    Polygon2d() = default;

    /**
     * Constructor from vertices automatically constructs edges
     */
    Polygon2d(const std::vector<Vertex2d>& vertices);

    /**
     * Returns the axis-aligned bounding box of this polygon
     */
    Polygon2d boundingBox();

    /**
     * List of vertices
     */
    std::vector<Vertex2d> vertices() const { return _vertices; }

    /**
     * List of edges
     */
    std::vector<Edge2d> edges() const { return _edges; }

    /**
     * Returns a copy of the polygon translated by (x,y) in meters and rotated by theta in radians
     */
    Polygon2d transform(double x, double y, double theta) const;

private:
    std::vector<Vertex2d> _vertices;
    std::vector<Edge2d> _edges;
};

bool collision(const Polygon2d& poly1, const Polygon2d& poly2);



struct Vertex3d
{
    double x, y, z;

    /**
     * Default constructor
     */
    Vertex3d() = default;

    /**
     * Constructor
     * @param x
     * @param y
     * @param z
     */
    Vertex3d(double x, double y, double z) : x(x), y(y), z(z) {};
};



struct Matrix3d
{
    double i0, j0, k0;
    double i1, j1, k1;
    double i2, j2, k2;

    /**
     * Default constructor
     */
    Matrix3d() = default;

    /**
     * Constructor from 2D position and orientation
     */
    Matrix3d(double x, double y, double theta) :
            i0(cos(theta)), j0(-sin(theta)), k0(x),
            i1(sin(theta)), j1(cos(theta)), k1(y),
            i2(0), j2(0), k2(1) {};

    Vertex3d operator*(const Vertex3d v)
    {
        return Vertex3d(i0*v.x + j0*v.y + k0*v.z, i1*v.x + j1*v.y + k1*v.z, i2*v.x + j2*v.y + k2*v.z);
    }
};

}


#endif //ROBOT_SIM_GEOMETRY_H
