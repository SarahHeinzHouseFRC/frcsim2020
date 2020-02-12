/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_VEHICLEMODEL_H
#define ROBOT_SIM_VEHICLEMODEL_H

#include "ConfigReader.h"
#include "Types.h"
#include "Geometry.h"
#include "BaseModel.h"


/**
 * Models the parameters of the robot. Enforces physics-based constraints on the joints and motors of the robot.
 */
class VehicleModel : public BaseModel
{
friend class VehicleView;
friend class Hud;
friend class PhysicsEngine;
public:
    /**
     * Constructor
     */
    VehicleModel(const ConfigReader& config, double startTimestamp);

    /**
     * Updates the robot model to the new time
     * @param currTimestamp Current time (sec). This must be greater than _lastTimestamp
     */
    void update(double currTimestamp);

    /**
     * Updates the robot's internal parameters given new commands
     * @param commands New commands
     */
    void processCommands(const CoreCommands& commands);

    /**
     * Returns state of all sensors onboard the vehicle
     */
    SensorState getSensorState();

    /**
     * Resets back to initial state
     */
    void reset();

    /**
     * Returns the bounding polygon of the vehicle in world coordinates
     */
    Geometry::Polygon2d polygon() const { return _boundingPolygonWorld; }

    /**
     * Returns the model type
     */
    virtual ModelType modelType() { return VEHICLE_MODEL; }

    /**
     * Collision callback
     */
    virtual void isInCollision(bool c) {};

private:
    /**
     * Bounds the given value to be no lower than min and no higher than max
     */
    static double bound(double val, double min, double max) { if (val < min) val = min; if (val > max) val = max; return val; }

    /**
     * Adds or subtracts 2*pi repeatedly to ensure 0 < val < 2*pi
     */
    static double wrapAngle(double val) { while (val > 2*M_PI) { val -= 2*M_PI; } while (val < 0) { val += 2*M_PI; } return val; }

    Geometry::Polygon2d _boundingPolygon; // Bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonWorld; // Bounding polygon of the vehicle in world frame
    Geometry::Polygon2d _ingestibleRegion; // Bounding polygon of the ingestible region in vehicle frame
    double _elevatorBeltLength; // Need to enforce the carriage to stay bw 0 and this belt length (meters)
    double _elevatorMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _leftDriveMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _rightDriveMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _elevatorMotorRadius; // Needed to calculate travel of belt per unit time
    double _wheelRadius; // Needed to calculate travel of robot per unit time
    double _wheelTrack; // Needed to calculate arced turns
    double _drivetrainWidth; // Needed to calculate turning radius
    double _mass; // Needed to calculate density for collision checker
    double _prevTimestamp; // Needed to calculate how much time has passed since last update()

    struct
    {
        double x, y, theta;
    } _initialState;

    struct VehicleState
    {
        double elevatorMotorSpeed; // Rads/sec, 0-maxElevatorMotorSpeed
        double elevatorCarriagePos; // Meters, 0-elevatorBeltLength
        double leftDriveMotorSpeed; // Rads/sec, 0-maxDriveSpeed
        double rightDriveMotorSpeed; // Rads/sec, 0-maxDriveSpeed
        struct
        {
            double x;     // Meters
            double y;     // Meters
            double vx;    // Meters/sec
            double vy;    // Meters/sec
            double omega; // Rads/sec
            double theta; // Rads
        } pose;
    } _state;
};


#endif //ROBOT_SIM_VEHICLEMODEL_H
