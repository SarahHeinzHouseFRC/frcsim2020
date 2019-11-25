/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include "ConfigReader.h"


ConfigReader::ConfigReader(const std::string &configFile)
{
    YAML::Node config = YAML::LoadFile(configFile);

    YAML::Node elevatorConfig = config["elevator"];
    if (elevatorConfig)
    {
        loadElevatorConfig(elevatorConfig);
    }

    std::cout << "Successfully loaded config file " << configFile << std::endl;
}



void ConfigReader::loadElevatorConfig(const YAML::Node& elevatorConfig)
{
    // Load belt
    YAML::Node beltConfig = elevatorConfig["belt"];
    if (beltConfig)
    {
        elevator.belt.radius = beltConfig["radius"].as<float>();
        elevator.belt.width = beltConfig["width"].as<float>();
        elevator.belt.length = beltConfig["length"].as<float>();
    }

    // Load motor shaft
    YAML::Node motorShaftConfig = elevatorConfig["motorShaft"];
    if (beltConfig)
    {
        elevator.motorShaft.radius = motorShaftConfig["radius"].as<float>();
        elevator.motorShaft.length = motorShaftConfig["length"].as<float>();
    }

    // Load motor
    YAML::Node motorConfig = elevatorConfig["motor"];
    if (motorConfig)
    {
        elevator.motor.radius = motorConfig["radius"].as<float>();
        elevator.motor.length = motorConfig["length"].as<float>();
        elevator.motor.maxSpeed = motorConfig["maxSpeed"].as<float>();
    }

    // Load encoder
    YAML::Node encoderConfig = elevatorConfig["encoder"];
    if (encoderConfig)
    {
        elevator.encoder.radius = encoderConfig["radius"].as<float>();
        elevator.encoder.length = encoderConfig["length"].as<float>();
    }

    // Load carriage
    YAML::Node carriageConfig = elevatorConfig["carriage"];
    if (carriageConfig)
    {
        elevator.carriage.lengthX = carriageConfig["lengthX"].as<float>();
        elevator.carriage.lengthY = carriageConfig["lengthY"].as<float>();
        elevator.carriage.lengthZ = carriageConfig["lengthZ"].as<float>();
    }
}
