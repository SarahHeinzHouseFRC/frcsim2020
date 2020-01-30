/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <iostream>
#include <ConfigReader.h>
#include "ConfigReader.h"

#define RPM_TO_RADS_PER_SEC 0.1047f
#define DEG_TO_RAD 0.0175f
#define IN_TO_M 0.0254f


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

    YAML::Node simConfig = config["sim"];
    if (simConfig)
    {
        parseSimConfig(simConfig);
    }

    std::cout << "Successfully loaded config file " << configFile << std::endl;
}



void ConfigReader::parseCoreConfig(const YAML::Node& coreConfig)
{
    core.ip = coreConfig["ip"].as<std::string>();
    core.joystickPort = coreConfig["joystickPort"].as<int>();
    core.vehiclePort = coreConfig["vehiclePort"].as<int>();
}



void ConfigReader::parseSimConfig(const YAML::Node& simConfig)
{
    sim.ip = simConfig["ip"].as<std::string>();
    sim.port = simConfig["port"].as<int>();
    parseSimConstantsConfig(simConfig["constants"]);
    parseSimInitialStateConfig(simConfig["initialState"]);
}



void ConfigReader::parseSimConstantsConfig(const YAML::Node& simConstantsConfig)
{
    //
    // Load drivetrain constant params
    //

    YAML::Node drivetrainConfig = simConstantsConfig["drivetrain"];

    if (drivetrainConfig)
    {
        sim.constants.drivetrain.width = drivetrainConfig["width"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.depth = drivetrainConfig["depth"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.widthChannel = drivetrainConfig["widthChannel"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.heightChannel = drivetrainConfig["heightChannel"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.wheelRadius = drivetrainConfig["wheelRadius"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.wheelWidth = drivetrainConfig["wheelWidth"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.wheelBase = drivetrainConfig["wheelBase"].as<float>() * IN_TO_M;
        sim.constants.drivetrain.wheelTrack = sim.constants.drivetrain.width - 2*sim.constants.drivetrain.widthChannel - sim.constants.drivetrain.wheelWidth;
        sim.constants.drivetrain.motor.maxSpeed = drivetrainConfig["motor"]["maxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
    }

    //
    // Load elevator constant params
    //

    YAML::Node elevatorConfig = simConstantsConfig["elevator"];

    if (elevatorConfig)
    {
        // Load belt
        YAML::Node beltConfig = elevatorConfig["belt"];
        if (beltConfig)
        {
            sim.constants.elevator.belt.radius = beltConfig["radius"].as<float>() * IN_TO_M;
            sim.constants.elevator.belt.width = beltConfig["width"].as<float>() * IN_TO_M;
            sim.constants.elevator.belt.length = beltConfig["length"].as<float>() * IN_TO_M;
        }

        // Load motor shaft
        YAML::Node motorShaftConfig = elevatorConfig["motorShaft"];
        if (beltConfig)
        {
            sim.constants.elevator.motorShaft.radius = motorShaftConfig["radius"].as<float>() * IN_TO_M;
            sim.constants.elevator.motorShaft.length = motorShaftConfig["length"].as<float>() * IN_TO_M;
        }

        // Load motor
        YAML::Node motorConfig = elevatorConfig["motor"];
        if (motorConfig)
        {
            sim.constants.elevator.motor.radius = motorConfig["radius"].as<float>() * IN_TO_M;
            sim.constants.elevator.motor.length = motorConfig["length"].as<float>() * IN_TO_M;
            sim.constants.elevator.motor.maxSpeed = motorConfig["maxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        }

        // Load encoder
        YAML::Node encoderConfig = elevatorConfig["encoder"];
        if (encoderConfig)
        {
            sim.constants.elevator.encoder.radius = encoderConfig["radius"].as<float>() * IN_TO_M;
            sim.constants.elevator.encoder.length = encoderConfig["length"].as<float>() * IN_TO_M;
        }

        // Load carriage
        YAML::Node carriageConfig = elevatorConfig["carriage"];
        if (carriageConfig)
        {
            sim.constants.elevator.carriage.lengthX = carriageConfig["lengthX"].as<float>() * IN_TO_M;
            sim.constants.elevator.carriage.lengthY = carriageConfig["lengthY"].as<float>() * IN_TO_M;
            sim.constants.elevator.carriage.lengthZ = carriageConfig["lengthZ"].as<float>() * IN_TO_M;
        }
    }

    //
    // Load game piece config
    //

    YAML::Node gamePieceConfig = simConstantsConfig["gamePiece"];
    if (gamePieceConfig)
    {
        sim.constants.gamePiece.radius = gamePieceConfig["radius"].as<float>() * IN_TO_M;
    }
}



void ConfigReader::parseSimInitialStateConfig(const YAML::Node& simInitialStateConfig)
{
    //
    // Load drivetrain initial state params
    //

    YAML::Node drivetrainConfig = simInitialStateConfig["drivetrain"];

    if (drivetrainConfig)
    {
        sim.initialState.drivetrain.x = drivetrainConfig["x"].as<float>() * IN_TO_M;
        sim.initialState.drivetrain.y = drivetrainConfig["y"].as<float>() * IN_TO_M;
        sim.initialState.drivetrain.theta = drivetrainConfig["theta"].as<float>() * DEG_TO_RAD;
    }

    //
    // Load elevator initial state params
    //

    YAML::Node elevatorConfig = simInitialStateConfig["elevator"];

    if (elevatorConfig)
    {
        sim.initialState.elevator.motorSpeed = elevatorConfig["motorSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        sim.initialState.elevator.carriagePos = elevatorConfig["carriagePos"].as<float>() * IN_TO_M;
    }
}
