/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include "ConfigReader.h"
#include "Types.h"
#include "Geometry.h"
#include "BaseModel.h"
#include "GamePieceModel.h"


/**
 * Models the parameters of the robot. Enforces physics-based constraints on the joints and motors of the robot.
 */
class VehicleModel : public BaseModel
{
friend class VehicleView;
friend class Hud;
friend class PhysicsEngine;
friend class WorldModel;
public:
    /**
     * Default constructor
     */
    VehicleModel() = default;

    /**
     * Constructor
     */
    VehicleModel(const ConfigReader& config, double startTimestamp, int playerId);

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
     * Clears all LIDAR points. Called once all points have been tx'd
     */
    void clearLidarPoints() { _state.lidarPoints.clear(); }

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

    std::string _team;
    std::string _alliance;
    Geometry::Polygon2d _boundingPolygonFrontLeft; // Front left bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonFrontRight; // Front right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonRearLeft; // Rear left bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonRearRight; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperFrontLeft; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperFrontRight; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperLeft; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperRight; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperRearLeft; // Rear left bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _boundingPolygonBumperRearRight; // Rear right bounding polygon of the vehicle in vehicle frame
    Geometry::Polygon2d _ingestibleRegionCenter; // Bounding polygon of the center ingestible region in vehicle frame
    Geometry::Polygon2d _tubeRegion; // Bounding polygon of the ingestion region in vehicle frame
    Geometry::Polygon2d _ingestibleRegionLeft; // Bounding polygon of the left ingestible region in vehicle frame
    Geometry::Polygon2d _ingestibleRegionRight; // Bounding polygon of the right ingestible region in vehicle frame
    double _leftDriveMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _rightDriveMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _intakeCenterMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _intakeLeftMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _intakeRightMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _tubeMotorMaxSpeed; // Need to enforce the motor speed to stay bw 0 and this max speed (rads/sec)
    double _wheelRadius; // Needed to calculate travel of robot per unit time
    double _wheelTrack; // Needed to calculate arced turns
    double _mass; // Needed to calculate density for collision checker
    double _prevTimestamp; // Needed to calculate how much time has passed since last update()
    bool _outtake; // Whether or not this vehicle has requested the field to outtake
    bool _prevOuttakeButtonState; // Whether or not the controller was previously requesting an outtake
    bool _hasLidar; // Whether or not this vehicle has a LIDAR sensor

    struct
    {
        double x, y, theta;
    } _initialState;

    struct VehicleControls
    {
        double leftDriveMotorSpeed; // Rads/sec, 0-leftDriveMotorMaxSpeed
        double rightDriveMotorSpeed; // Rads/sec, 0-rightDriveMotorMaxSpeed
        double intakeCenterMotorSpeed; // Rads/sec, 0-intakeCenterMotorMaxSpeed
        double intakeLeftMotorSpeed; // Rads/sec, 0-intakeLeftMotorMaxSpeed
        double intakeRightMotorSpeed; // Rads/sec, 0-intakeRightMotorMaxSpeed
        double tubeMotorSpeed; // Rads/sec, 0-tubeMotorMaxSpeed
    } _controls;

    struct VehicleState
    {
        double x;                            // Meters
        double y;                            // Meters
        double theta;                        // Rads
        double vx;                           // Meters/sec
        double vy;                           // Meters/sec
        double omega;                        // Rads/sec
        int numIngestedBalls;                // Number of balls in tube
        std::vector<LidarPoint> lidarPoints; // LIDAR points
    } _state;
};
