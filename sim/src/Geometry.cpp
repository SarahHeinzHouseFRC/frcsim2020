/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "Geometry.h"

namespace Geometry
{

int orientation(const Vertex2d& a, const Vertex2d& b, const Vertex2d& c)
{
    // Reference: https://www.geeksforgeeks.org/orientation-3-ordered-points/

    // Collinear: val == 0
    // Clockwise: val > 0
    // Counterclockwise: val < 0
    double val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0) { return val; }
    else { return (val > 0)? 1 : 2; }
}



bool onSegment(const Vertex2d& a, const Vertex2d& b, const Vertex2d& c)
{
    return b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) &&
           b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y);
}



bool collision(const Edge2d& edge1, const Edge2d& edge2)
{
    // Reference: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

    Vertex2d a = edge1.a;
    Vertex2d b = edge1.b;
    Vertex2d c = edge2.a;
    Vertex2d d = edge2.b;

    int o1 = orientation(a, b, c);
    int o2 = orientation(a, b, d);
    int o3 = orientation(c, d, a);
    int o4 = orientation(c, d, b);

    // Special cases
    // a, b and c are collinear and c lies on segment ab
    if (o1 == 0 && onSegment(a, c, b))
    {
        return true;
    }

    // a, b and d are collinear and d lies on segment ab
    if (o2 == 0 && onSegment(a, d, b))
    {
        return true;
    }

    // c, d and a are collinear and a lies on segment cd
    if (o3 == 0 && onSegment(c, a, d))
    {
        return true;
    }

    // c, d and b are collinear and b lies on segment cd
    if (o4 == 0 && onSegment(c, b, d))
    {
        return true;
    }

    // General case
    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    // General case
    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    // No intersections
    return false;
}



Polygon2d::Polygon2d(const std::vector<Vertex2d>& vertices) : _vertices(vertices)
{
    // Construct edges
    for (int i=0; i<_vertices.size(); i++)
    {
        int j = (i + 1) % _vertices.size();
        Edge2d edge{ _vertices[i], _vertices[j] };
        _edges.push_back(edge);
    }
}



Polygon2d Polygon2d::boundingBox()
{
    double minX = _vertices[0].x;
    double maxX = _vertices[0].x;
    double minY = _vertices[0].y;
    double maxY = _vertices[0].y;
    for (const auto& vertex : _vertices)
    {
        if (minX > vertex.x)
        {
            minX = vertex.x;
        }
        if (maxX < vertex.x)
        {
            maxX = vertex.x;
        }
        if (minY > vertex.y)
        {
            minY = vertex.y;
        }
        if (maxY < vertex.y)
        {
            maxY = vertex.y;
        }
    }

    return Polygon2d(std::vector<Vertex2d>({ { minX, minY }, { maxX, minY }, { maxX, maxY }, { minX, maxY } }));
}



Polygon2d Polygon2d::transform(double x, double y, double theta) const
{
    std::vector<Vertex2d> result;
    for (const auto& vertex : _vertices)
    {
        Vertex3d vertexVehicleFrame(vertex.x, vertex.y, 1);
        Matrix3d vehicleToWorld(x, y, theta);
        Vertex3d vertexWorldFrame = vehicleToWorld * vertexVehicleFrame;
        result.push_back(Vertex2d(vertexWorldFrame.x, vertexWorldFrame.y));
    }
    return Polygon2d(result);
}



bool collision(const Polygon2d& poly1, const Polygon2d& poly2)
{
    // Brute-force O(n^2) segment-segment check of both polygons
    for (const auto& segment1 : poly1.edges())
    {
        for (const auto& segment2 : poly2.edges())
        {
            bool c = collision(segment1, segment2);
            if (c) { return c; }
        }
    }

    return false;
}

}