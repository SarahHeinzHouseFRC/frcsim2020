/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_CONFIGREADER_H
#define ROBOT_SIM_CONFIGREADER_H

#include <string>
#include <yaml-cpp/yaml.h>


/**
 * Parses and converts the values in a config file and allows public access to its fields to other classes. Note that
 * despite the config file specifying values in Imperial units, the sim uses SI units throughout. Some values provided
 * by this class are not directly read from the config file, but are calculated from other fields for convenient access.
 */
class ConfigReader
{
public:
    /**
     * Default constructor
     */
    ConfigReader();

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
        std::string ip; // IP address
        int port;       // Port number
        struct
        {
            struct
            {
                float width;         // Meters
                float depth;         // Meters
                float widthChannel;  // Meters
                float heightChannel; // Meters
                float wheelRadius;   // Meters
                float wheelWidth;    // Meters
                float wheelBase;     // Meters
                float wheelTrack;    // Meters (calculated value)
                struct
                {
                    float maxSpeed; // Rads/sec
                } motor;
            } drivetrain;
            struct
            {
                struct
                {
                    float radius; // Meters
                    float width;  // Meters
                    float length; // Meters
                } belt;
                struct
                {
                    float radius; // Meters
                    float length; // Meters
                } motorShaft;
                struct
                {
                    float radius;   // Meters
                    float length;   // Meters
                    float maxSpeed; // Rads/sec
                } motor;
                struct
                {
                    float radius; // Meters
                    float length; // Meters
                } encoder;
                struct
                {
                    float lengthX; // Meters
                    float lengthY; // Meters
                    float lengthZ; // Meters
                } carriage;
            } elevator;
            struct
            {
                float radius; // Meters
            } gamePiece;
        } constants;
        struct
        {
            struct
            {
                float x;     // Meters
                float y;     // Meters
                float theta; // Degrees
            } drivetrain;
            struct
            {
                float motorSpeed;  // Rads/sec
                float carriagePos; // Meters
            } elevator;
        } initialState;
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
     * Helper method for loading sim constant parameters
     */
    void parseSimConstantsConfig(const YAML::Node& simConstantsConfig);

    /**
     * Helper method for loading sim initial state parameters
     */
    void parseSimInitialStateConfig(const YAML::Node& simInitialStateConfig);
};


#endif //ROBOT_SIM_CONFIGREADER_H
