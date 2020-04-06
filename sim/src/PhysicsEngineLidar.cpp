/**
 * Copyright (c) 2020 Team 3260
 */

#include "PhysicsEngineLidar.h"

#define NUM_RAYS_PER_SWEEP 800


PhysicsEngineLidar::PhysicsEngineLidar(b2World* world) : _world(world)
{

}



std::vector<LidarPoint> PhysicsEngineLidar::sweep(const b2Transform& pose)
{
    std::vector<LidarPoint> results(NUM_RAYS_PER_SWEEP);

    constexpr double minRange = 0.75;
    constexpr double maxRange = 12;
    for (int i=0; i<NUM_RAYS_PER_SWEEP; i++)
    {
        double azimuth = i * (2*M_PI) / NUM_RAYS_PER_SWEEP;
        b2Vec2 nearPoint = b2Mul(pose, b2Vec2(minRange * cos(azimuth), minRange * sin(azimuth)));
        b2Vec2 farPoint =  b2Mul(pose, b2Vec2(maxRange * cos(azimuth), maxRange * sin(azimuth)));

        LidarRayCastCallback rayCastCallback(minRange, maxRange);
        _world->RayCast(&rayCastCallback, nearPoint, farPoint);
        LidarPoint p{};
        p.azimuth = azimuth;
        p.elevation = 0;
        p.range = rayCastCallback.hasIntersections() ? rayCastCallback.getRange() : -1;
        results.at(i) =  p;
    }

    return results;
}
