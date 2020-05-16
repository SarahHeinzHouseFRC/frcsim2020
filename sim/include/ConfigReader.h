/**
 * Copyright (c) 2020 FRC Team 3260
 */

#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include "Geometry.h"


/**
 * Parses and converts the values in a config file and allows public access to its fields to other classes. Note that
 * despite the config file specifying values in Imperial units, the sim uses SI units throughout. Some values provided
 * by this class are not directly read from the config file, but are calculated from other fields for convenient access.
 * Also note that weights are converted into masses by this class.
 */
class ConfigReader
{
public:
    /**
     * Default constructor
     */
    ConfigReader() = default;

    /**
     * Parses the given config file and stores its values as public fields within this class
     */
    void parse(const std::string& configFile);

    struct Pose
    {
        float x;
        float y;
        float theta;
    };

    struct Player
    {
        std::string team;     // Team number
        std::string alliance; // "Blue" or "Red"
        Pose initialPosition; // Initial pose
        bool hasLidar;        // Has LIDAR or not
    };

    std::vector<Player> players; // List of all possible players
    struct
    {
        std::string ip;   // IP address
        int joystickPort; // Port number
        int simPort;      // Port number
    } core;
    struct
    {
        struct
        {
            std::string ip;  // IP address
            int corePort;    // Port number
            int simViewPort; // Port number
        } comms;
        struct
        {
            std::string fieldModelFile;       // Path
            std::string vehicleBlueModelFile; // Path
            std::string vehicleRedModelFile;  // Path
            std::string gamePieceModelFile;   // Path
            std::string fontFile;             // Path
        } assets;
        struct
        {
            Geometry::Polygon2d exteriorWall;
            Geometry::Polygon2d rightTrenchRightWall;
            Geometry::Polygon2d rightTrenchLeftWall;
            Geometry::Polygon2d leftTrenchRightWall;
            Geometry::Polygon2d leftTrenchLeftWall;
            Geometry::Polygon2d rightColumn;
            Geometry::Polygon2d topColumn;
            Geometry::Polygon2d leftColumn;
            Geometry::Polygon2d bottomColumn;
            Geometry::Polygon2d blueGoalRegion;
            Geometry::Polygon2d redGoalRegion;
            Geometry::Vertex2d blueOuttake;
            Geometry::Vertex2d redOuttake;
        } field;
        struct
        {
            Geometry::Polygon2d boundingPolygonFrontLeft;        // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonFrontRight;       // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonRearLeft;         // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonRearRight;        // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperFrontLeft;  // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperFrontRight; // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperLeft;       // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperRight;      // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperRearLeft;   // Meters in vehicle frame
            Geometry::Polygon2d boundingPolygonBumperRearRight;  // Meters in vehicle frame
            Geometry::Polygon2d ingestibleRegionCenter;          // Meters in vehicle frame
            Geometry::Polygon2d ingestibleRegionLeft;            // Meters in vehicle frame
            Geometry::Polygon2d ingestibleRegionRight;           // Meters in vehicle frame
            Geometry::Polygon2d tubeRegion;                      // Meters in vehicle frame
            float mass;                                                      // Kilograms
            struct
            {
                float wheelBase;          // Meters
                float wheelTrack;         // Meters
                float wheelRadius;        // Meters
                float wheelWidth;         // Meters
                float leftMotorMaxSpeed;  // Rads/sec
                float rightMotorMaxSpeed; // Rads/sec
            } drivetrain;
            struct
            {
                float centerMotorMaxSpeed; // Rads/sec
                float leftMotorMaxSpeed;   // Rads/sec
                float rightMotorMaxSpeed;  // Rads/sec
                float tubeMotorMaxSpeed;   // Rads/sec
            } intake;
            struct
            {
                float minRange;       // Meters
                float maxRange;       // Meters
                float laserFrequency; // Hertz
                float motorFrequency; // Hertz
            } lidar;
        } vehicle;
        struct
        {
            float radius;                                     // Meters
            std::vector<Geometry::Vertex2d> initialPositions; // Meters (where (0, 0) is the center of the field)
        } gamePiece;
    } sim;
    struct
    {
        std::string ip; // IP address
        int port;       // Port number
    } simView;
    bool verbose;
    bool debugView;
    bool headless;

private:
    /**
     * Helper method for loading player parameters
     */
    void parsePlayersConfig(const YAML::Node& playersConfig);

    /**
     * Helper method for loading core parameters
     */
    void parseCoreConfig(const YAML::Node& coreConfig);

    /**
     * Helper method for loading sim parameters
     */
    void parseSimConfig(const YAML::Node& simConfig);

    /**
     * Helper-helper method for loading sim comms config parameters
     */
    void parseSimCommsConfig(const YAML::Node& commsConfig);

    /**
     * Helper-helper method for loading sim asset config parameters
     */
    void parseSimAssetsConfig(const YAML::Node& assetsConfig);

    /**
     * Helper-helper method for loading sim field config parameters
     */
    void parseSimFieldConfig(const YAML::Node& fieldConfig);

    /**
     * Helper-helper method for loading sim vehicle config parameters
     */
    void parseSimVehicleConfig(const YAML::Node& vehicleConfig);

    /**
     * Helper-helper method for loading sim game piece config parameters
     */
    void parseSimGamePieceConfig(const YAML::Node& gamePieceConfig);

    /**
     * Helper method for loading sim view parameters
     */
    void parseSimViewConfig(const YAML::Node& simViewConfig);

    /**
     * Helper method for parsing a polygon
     */
    static std::vector<Geometry::Vertex2d> parsePolygon(const YAML::Node& node);

    /**
     * Helper method for parsing a vertex
     */
    static Geometry::Vertex2d parseVertex(const YAML::Node& node);
};
