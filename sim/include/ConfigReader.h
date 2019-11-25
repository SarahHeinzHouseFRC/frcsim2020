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
    ConfigReader(const std::string& configFile);

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

public:
    void loadElevatorConfig(const YAML::Node& elevatorConfig);
};


#endif //ROBOT_SIM_CONFIGREADER_H
