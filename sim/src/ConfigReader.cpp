/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include <ConfigReader.h>
#include "ConfigReader.h"


ConfigReader::ConfigReader() = default;



void ConfigReader::parse(const std::string &configFile)
{
    YAML::Node config;
    try
    {
        config = YAML::LoadFile(configFile);
    }
    catch(YAML::BadFile& e)
    {
        throw std::runtime_error("ConfigReader: File not found: " + configFile);
    }

    YAML::Node coreConfig = config["core"];
    if (coreConfig)
    {
        parseCoreConfig(coreConfig);
    }

    YAML::Node vehicleConfig = config["vehicle"];
    if (vehicleConfig)
    {
        parseVehicleConfig(vehicleConfig);
    }

    std::cout << "Successfully loaded config file " << configFile << std::endl;
}



void ConfigReader::parseCoreConfig(const YAML::Node& coreConfig)
{
    core.ip = coreConfig["ip"].as<std::string>();
    core.joystickPort = coreConfig["joystickPort"].as<int>();
    core.vehiclePort = coreConfig["vehiclePort"].as<int>();
}



void ConfigReader::parseVehicleConfig(const YAML::Node& vehicleConfig)
{
    vehicle.ip = vehicleConfig["ip"].as<std::string>();
    vehicle.port = vehicleConfig["port"].as<int>();
    parseVehicleConstantsConfig(vehicleConfig["constants"]);
    parseVehicleInitialStateConfig(vehicleConfig["initialState"]);
}



void ConfigReader::parseVehicleConstantsConfig(const YAML::Node& vehicleConstantsConfig)
{
    //
    // Load elevator constant params
    //

    YAML::Node elevatorConfig = vehicleConstantsConfig["elevator"];

    if (elevatorConfig)
    {
        // Load belt
        YAML::Node beltConfig = elevatorConfig["belt"];
        if (beltConfig)
        {
            vehicle.constants.elevator.belt.radius = beltConfig["radius"].as<float>();
            vehicle.constants.elevator.belt.width = beltConfig["width"].as<float>();
            vehicle.constants.elevator.belt.length = beltConfig["length"].as<float>();
        }

        // Load motor shaft
        YAML::Node motorShaftConfig = elevatorConfig["motorShaft"];
        if (beltConfig)
        {
            vehicle.constants.elevator.motorShaft.radius = motorShaftConfig["radius"].as<float>();
            vehicle.constants.elevator.motorShaft.length = motorShaftConfig["length"].as<float>();
        }

        // Load motor
        YAML::Node motorConfig = elevatorConfig["motor"];
        if (motorConfig)
        {
            vehicle.constants.elevator.motor.radius = motorConfig["radius"].as<float>();
            vehicle.constants.elevator.motor.length = motorConfig["length"].as<float>();
            vehicle.constants.elevator.motor.maxSpeed = motorConfig["maxSpeed"].as<float>();
        }

        // Load encoder
        YAML::Node encoderConfig = elevatorConfig["encoder"];
        if (encoderConfig)
        {
            vehicle.constants.elevator.encoder.radius = encoderConfig["radius"].as<float>();
            vehicle.constants.elevator.encoder.length = encoderConfig["length"].as<float>();
        }

        // Load carriage
        YAML::Node carriageConfig = elevatorConfig["carriage"];
        if (carriageConfig)
        {
            vehicle.constants.elevator.carriage.lengthX = carriageConfig["lengthX"].as<float>();
            vehicle.constants.elevator.carriage.lengthY = carriageConfig["lengthY"].as<float>();
            vehicle.constants.elevator.carriage.lengthZ = carriageConfig["lengthZ"].as<float>();
        }
    }
}



void ConfigReader::parseVehicleInitialStateConfig(const YAML::Node& vehicleInitialStateConfig)
{
    //
    // Load elevator initial state params
    //

    YAML::Node elevatorConfig = vehicleInitialStateConfig["elevator"];

    if (elevatorConfig)
    {
        vehicle.initialState.elevator.motorSpeed = elevatorConfig["motorSpeed"].as<float>();
        vehicle.initialState.elevator.carriagePos = elevatorConfig["carriagePos"].as<float>();
    }
}
