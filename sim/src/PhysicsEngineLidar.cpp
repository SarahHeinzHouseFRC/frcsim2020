/**
 * Copyright (c) 2020 Team 3260
 */

#include "ConfigReader.h"
#include "PhysicsEngineLidar.h"


PhysicsEngineLidar::PhysicsEngineLidar(b2World* world, double timestamp, const ConfigReader& config) :
        _prevTimestamp(timestamp),
        _world(world),
        _minRange(config.sim.vehicle.lidar.minRange),
        _maxRange(config.sim.vehicle.lidar.maxRange),
        _laserFrequency(config.sim.vehicle.lidar.laserFrequency),
        _motorFrequency(config.sim.vehicle.lidar.motorFrequency),
        _azimuth(0)
{

}



std::vector<LidarPoint> PhysicsEngineLidar::sweep(const b2Transform& pose, double timestamp)
{
    double elapsedTime = timestamp - _prevTimestamp;
    int numRays = elapsedTime * _laserFrequency;
    double deltaTheta = 2*M_PI * _motorFrequency / _laserFrequency;

    std::vector<LidarPoint> results(numRays);

    for (int i=0; i<numRays; i++)
    {
        _azimuth += deltaTheta;
        if (_azimuth > 2*M_PI) { _azimuth -= 2*M_PI; }
        b2Vec2 nearPoint = b2Mul(pose, b2Vec2(_minRange * cos(_azimuth), _minRange * sin(_azimuth)));
        b2Vec2 farPoint =  b2Mul(pose, b2Vec2(_maxRange * cos(_azimuth), _maxRange * sin(_azimuth)));

        LidarRayCastCallback rayCastCallback(_minRange, _maxRange);
        _world->RayCast(&rayCastCallback, nearPoint, farPoint);
        LidarPoint p{};
        p.azimuth = _azimuth;
        p.elevation = 0;
        p.range = rayCastCallback.hasIntersections() ? rayCastCallback.getRange() : -1;
        results.at(i) =  p;
    }

    _prevTimestamp = timestamp;

    return results;
}
