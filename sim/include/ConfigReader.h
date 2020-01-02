/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_CONFIGREADER_H
#define ROBOT_SIM_CONFIGREADER_H

#include <string>
#include <yaml-cpp/yaml.h>


/**
 * Reads and parses the a config file, then provides its values to other classes.
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
                float width;         // Inches
                float depth;         // Inches
                float widthChannel;  // Inches
                float heightChannel; // Inches
                float wheelRadius;   // Inches
                float wheelWidth;    // Inches
                float wheelBase;     // Inches
                float wheelTrack;    // Inches (calculated value)
                struct
                {
                    float maxSpeed; // RPM
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
                    float maxSpeed; // RPM
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
        } constants;
        struct
        {
            struct
            {
                float x; // Meters
                float y; // Meters
            } drivetrain;
            struct
            {
                float motorSpeed;  // RPM
                float carriagePos; // Meters
            } elevator;
        } initialState;
    } vehicle;

private:
    /**
     * Helper method for loading controls parameters
     */
    void parseCoreConfig(const YAML::Node& coreConfig);

    /**
     * Helper method for loading vehicle parameters
     */
    void parseVehicleConfig(const YAML::Node& simConfig);

    /**
     * Helper method for loading vehicle constant parameters
     */
    void parseVehicleConstantsConfig(const YAML::Node& vehicleConstantsConfig);

    /**
     * Helper method for loading vehicle initial state parameters
     */
    void parseVehicleInitialStateConfig(const YAML::Node& vehicleInitialStateConfig);
};


#endif //ROBOT_SIM_CONFIGREADER_H
