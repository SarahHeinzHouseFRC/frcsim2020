/**
 * Copyright (c) 2020 FRC Team 3260
 */

#ifndef ROBOT_SIM_CONFIGREADER_H
#define ROBOT_SIM_CONFIGREADER_H

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

    struct
    {
        std::string ip;   // IP address
        int joystickPort; // Port number
        int vehiclePort;  // Port number
    } core;
    struct
    {
        struct
        {
            std::string ip; // IP address
            int port;       // Port number
        } comms;
        struct
        {
            std::string fieldModelFile;     // Path
            std::string vehicleModelFile;   // Path
            std::string gamePieceModelFile; // Path
            std::string fontFile;           // Path
        } assets;
        struct
        {
            std::vector<Geometry::Vertex2d> exteriorPolygon;
            std::vector<std::vector<Geometry::Vertex2d>> interiorPolygons;
        } field;
        struct
        {
            std::vector<Geometry::Vertex2d> polygon; // Meters where (0,0) is CoG of the vehicle
            float mass;                              // Kilograms
            struct
            {
                float x;     // Meters
                float y;     // Meters
                float theta; // Degrees
            } initialState;
            struct
            {
                float width;              // Meters
                float depth;              // Meters
                float widthChannel;       // Meters
                float heightChannel;      // Meters
                float wheelRadius;        // Meters
                float wheelWidth;         // Meters
                float wheelBase;          // Meters
                float wheelTrack;         // Meters (derived value)
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
        } vehicle;
        struct
        {
            float radius;                                     // Meters
            std::vector<Geometry::Vertex2d> initialPositions; // Meters (where (0, 0) is the center of the field)
        } gamePiece;
    } sim;
    bool verbose;
    bool debugView;

private:
    /**
     * Helper method for loading controls parameters
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
};


#endif //ROBOT_SIM_CONFIGREADER_H
