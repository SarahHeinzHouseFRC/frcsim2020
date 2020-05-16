/**
 * Copyright (c) 2020 Team 3260
 */

#pragma once

#include <vector>
#include "Box2D/Box2D.h"
#include "Types.h"


/**
 * Range: 0.15 - 12m
 * Rotation speed: 600 rpm
 * Scanning freq: 8000 Hz
 * => One rotation: 0.1s, 800 points
 */
class PhysicsEngineLidar
{
public:
    /**
     * Constructor
     */
    PhysicsEngineLidar(b2World* world, const ConfigReader& config);

    /**
     * Performs one sweep of the LIDAR from the given vehicle pose
     */
    std::vector<LidarPoint> sweep(const b2Transform &pose);

private:
    class LidarRayCastCallback : public b2RayCastCallback
    {
    public:
        LidarRayCastCallback(double minRange, double maxRange) :
                _fraction(0),
                _numIntersections(0),
                _minRange(minRange),
                _maxRange(maxRange) {};

        float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
        {
            _numIntersections++;
            _fraction = fraction;
            return fraction; // Keep shrinking the ray so we get the closest intersection
        }

        /**
         * Returns the range in meters
         */
        double getRange() { return _fraction * (_maxRange - _minRange) + _minRange; }

        /**
         * Returns whether the ray had any intersections
         */
        int hasIntersections() { return _numIntersections > 0; }
    private:
        double _minRange, _maxRange;
        int _numIntersections;
        float _fraction;
    };

    b2World* _world; // Non-owning pointer to world
    float _minRange;
    float _maxRange;
    float _numRaysPerSweep;
};
