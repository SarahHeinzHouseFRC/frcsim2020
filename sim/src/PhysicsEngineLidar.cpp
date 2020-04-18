/**
 * Copyright (c) 2020 Team 3260
 */

#include "ConfigReader.h"
#include "PhysicsEngineLidar.h"


PhysicsEngineLidar::PhysicsEngineLidar(b2World* world, const ConfigReader& config) :
        _world(world),
        _minRange(config.sim.vehicle.lidar.minRange),
        _maxRange(config.sim.vehicle.lidar.maxRange),
        _numRaysPerSweep(config.sim.vehicle.lidar.laserFrequency / config.sim.vehicle.lidar.motorFrequency)
{

}



std::vector<LidarPoint> PhysicsEngineLidar::sweep(const b2Transform& pose)
{
    std::vector<LidarPoint> results(_numRaysPerSweep);

    for (int i=0; i<_numRaysPerSweep; i++)
    {
        double azimuth = i * (2*M_PI) / _numRaysPerSweep;
        b2Vec2 nearPoint = b2Mul(pose, b2Vec2(_minRange * cos(azimuth), _minRange * sin(azimuth)));
        b2Vec2 farPoint =  b2Mul(pose, b2Vec2(_maxRange * cos(azimuth), _maxRange * sin(azimuth)));

        LidarRayCastCallback rayCastCallback(_minRange, _maxRange);
        _world->RayCast(&rayCastCallback, nearPoint, farPoint);
        LidarPoint p{};
        p.azimuth = azimuth;
        p.elevation = 0;
        p.range = rayCastCallback.hasIntersections() ? rayCastCallback.getRange() : -1;
        results.at(i) =  p;
    }

    return results;
}
