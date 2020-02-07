/**
 * Copyright (c) 2020 Team 3260
 */

#include <iostream>
#include "ConfigReader.h"

#define RPM_TO_RADS_PER_SEC 0.1047f
#define DEG_TO_RAD 0.0175f
#define IN_TO_M 0.0254f
#define LBS_TO_KG 0.453f


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
    parseSimCommsConfig(simConfig["comms"]);
    parseSimFieldConfig(simConfig["field"]);
    parseSimVehicleConfig(simConfig["vehicle"]);
    parseSimGamePieceConfig(simConfig["gamePiece"]);
}



void ConfigReader::parseSimCommsConfig(const YAML::Node& commsConfig)
{
    sim.comms.ip = commsConfig["ip"].as<std::string>();
    sim.comms.port = commsConfig["port"].as<int>();
}



void ConfigReader::parseSimFieldConfig(const YAML::Node& fieldConfig)
{
    YAML::Node exteriorPolygon = fieldConfig["exteriorPolygon"];
    for (const auto& vertex : exteriorPolygon)
    {
        sim.field.exteriorPolygon.emplace_back(vertex["x"].as<float>()*IN_TO_M, vertex["y"].as<float>()*IN_TO_M);
    }
    YAML::Node interiorPolygons = fieldConfig["interiorPolygons"];
    for (const auto& interiorPolygon : interiorPolygons)
    {
        std::vector<Geometry::Vertex2d> interiorVertices;
        for (const auto& vertex : interiorPolygon)
        {
            interiorVertices.emplace_back(vertex["x"].as<float>()*IN_TO_M, vertex["y"].as<float>()*IN_TO_M);
        }
        sim.field.interiorPolygons.emplace_back(interiorVertices);
    }
}



void ConfigReader::parseSimVehicleConfig(const YAML::Node& vehicleConfig)
{
    //
    // Load vehicle params
    //

    YAML::Node polygon = vehicleConfig["polygon"];
    for (auto it=polygon.begin(); it!=polygon.end(); it++)
    {
        YAML::Node point = *it;
        sim.vehicle.polygon.emplace_back(point["x"].as<float>()*IN_TO_M, point["y"].as<float>()*IN_TO_M);
    }
    sim.vehicle.mass = vehicleConfig["weight"].as<float>() * LBS_TO_KG;

    YAML::Node initialState = vehicleConfig["initialState"];

    if (initialState)
    {
        sim.vehicle.initialState.x = initialState["x"].as<float>() * IN_TO_M;
        sim.vehicle.initialState.y = initialState["y"].as<float>() * IN_TO_M;
        sim.vehicle.initialState.theta = initialState["theta"].as<float>() * DEG_TO_RAD;
    }

    //
    // Load vehicle drivetrain params
    //

    YAML::Node drivetrainConfig = vehicleConfig["drivetrain"];

    if (drivetrainConfig)
    {
        sim.vehicle.drivetrain.width = drivetrainConfig["width"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.depth = drivetrainConfig["depth"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.widthChannel = drivetrainConfig["widthChannel"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.heightChannel = drivetrainConfig["heightChannel"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.wheelRadius = drivetrainConfig["wheelRadius"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.wheelWidth = drivetrainConfig["wheelWidth"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.wheelBase = drivetrainConfig["wheelBase"].as<float>() * IN_TO_M;
        sim.vehicle.drivetrain.wheelTrack = sim.vehicle.drivetrain.width - 2*sim.vehicle.drivetrain.widthChannel - sim.vehicle.drivetrain.wheelWidth;
        sim.vehicle.drivetrain.motor.maxSpeed = drivetrainConfig["motor"]["maxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
    }

    //
    // Load elevator constant params
    //

    YAML::Node elevatorConfig = vehicleConfig["elevator"];

    if (elevatorConfig)
    {
        // Initial state
        YAML::Node initialStateConfig = elevatorConfig["initialState"];
        if (initialStateConfig)
        {
            sim.vehicle.elevator.initialState.motorSpeed = initialStateConfig["motorSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
            sim.vehicle.elevator.initialState.carriagePos = initialStateConfig["carriagePos"].as<float>() * IN_TO_M;
        }

        // Load belt
        YAML::Node beltConfig = elevatorConfig["belt"];
        if (beltConfig)
        {
            sim.vehicle.elevator.belt.radius = beltConfig["radius"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.belt.width = beltConfig["width"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.belt.length = beltConfig["length"].as<float>() * IN_TO_M;
        }

        // Load motor shaft
        YAML::Node motorShaftConfig = elevatorConfig["motorShaft"];
        if (beltConfig)
        {
            sim.vehicle.elevator.motorShaft.radius = motorShaftConfig["radius"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.motorShaft.length = motorShaftConfig["length"].as<float>() * IN_TO_M;
        }

        // Load motor
        YAML::Node motorConfig = elevatorConfig["motor"];
        if (motorConfig)
        {
            sim.vehicle.elevator.motor.radius = motorConfig["radius"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.motor.length = motorConfig["length"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.motor.maxSpeed = motorConfig["maxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        }

        // Load encoder
        YAML::Node encoderConfig = elevatorConfig["encoder"];
        if (encoderConfig)
        {
            sim.vehicle.elevator.encoder.radius = encoderConfig["radius"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.encoder.length = encoderConfig["length"].as<float>() * IN_TO_M;
        }

        // Load carriage
        YAML::Node carriageConfig = elevatorConfig["carriage"];
        if (carriageConfig)
        {
            sim.vehicle.elevator.carriage.lengthX = carriageConfig["lengthX"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.carriage.lengthY = carriageConfig["lengthY"].as<float>() * IN_TO_M;
            sim.vehicle.elevator.carriage.lengthZ = carriageConfig["lengthZ"].as<float>() * IN_TO_M;
        }
    }
}



void ConfigReader::parseSimGamePieceConfig(const YAML::Node& gamePieceConfig)
{
    if (gamePieceConfig)
    {
        sim.gamePiece.radius = gamePieceConfig["radius"].as<float>() * IN_TO_M;
        YAML::Node initialPositions = gamePieceConfig["initialPositions"];
        for (const auto& position : initialPositions)
        {
            sim.gamePiece.initialPositions.emplace_back(position["x"].as<float>()*IN_TO_M, position["y"].as<float>()*IN_TO_M);
        }
    }
}
