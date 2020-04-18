/**
 * Copyright (c) 2020 Team 3260
 */

#include <iostream>
#include <ConfigReader.h>
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

    YAML::Node playersConfig = config["players"];
    if (playersConfig)
    {
        parsePlayersConfig(playersConfig);
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

    YAML::Node simViewConfig = config["simView"];
    if (simViewConfig)
    {
        parseSimViewConfig(simViewConfig);
    }

    std::cout << "Successfully loaded config file " << configFile << std::endl;
}



void ConfigReader::parsePlayersConfig(const YAML::Node& playersConfig)
{
    for (const auto& playerConfig : playersConfig)
    {
        Player p;
        p.team = playerConfig["team"].as<std::string>();
        p.alliance = playerConfig["alliance"].as<std::string>();
        p.initialPosition.x = playerConfig["initialPosition"]["x"].as<float>() * IN_TO_M;
        p.initialPosition.y = playerConfig["initialPosition"]["y"].as<float>() * IN_TO_M;
        p.initialPosition.theta = playerConfig["initialPosition"]["theta"].as<float>() * DEG_TO_RAD;
        p.hasLidar = playerConfig["hasLidar"].as<bool>();
        players.push_back(p);
    }
}



void ConfigReader::parseCoreConfig(const YAML::Node& coreConfig)
{
    core.ip = coreConfig["ip"].as<std::string>();
    core.joystickPort = coreConfig["joystickPort"].as<int>();
    core.simPort = coreConfig["simPort"].as<int>();
}



void ConfigReader::parseSimConfig(const YAML::Node& simConfig)
{
    parseSimCommsConfig(simConfig["comms"]);
    parseSimAssetsConfig(simConfig["assets"]);
    parseSimFieldConfig(simConfig["field"]);
    parseSimVehicleConfig(simConfig["vehicle"]);
    parseSimGamePieceConfig(simConfig["gamePiece"]);
}



void ConfigReader::parseSimCommsConfig(const YAML::Node& commsConfig)
{
    sim.comms.ip = commsConfig["ip"].as<std::string>();
    sim.comms.corePort = commsConfig["corePort"].as<int>();
    sim.comms.simViewPort = commsConfig["simViewPort"].as<int>();
}



void ConfigReader::parseSimAssetsConfig(const YAML::Node &assetsConfig)
{
    sim.assets.fieldModelFile = assetsConfig["fieldModelFile"].as<std::string>();
    sim.assets.vehicleModelFile = assetsConfig["vehicleModelFile"].as<std::string>();
    sim.assets.gamePieceModelFile = assetsConfig["gamePieceModelFile"].as<std::string>();
    sim.assets.fontFile = assetsConfig["fontFile"].as<std::string>();
}



void ConfigReader::parseSimFieldConfig(const YAML::Node& fieldConfig)
{
    sim.field.exteriorPolygon = parsePolygon(fieldConfig["exteriorPolygon"]);
    for (const auto& interiorPolygon : fieldConfig["interiorPolygons"])
    {
        sim.field.interiorPolygons.emplace_back(parsePolygon(interiorPolygon));
    }
    sim.field.blueGoalPolygon = parsePolygon(fieldConfig["blueGoalPolygon"]);
    sim.field.redGoalPolygon = parsePolygon(fieldConfig["redGoalPolygon"]);
    sim.field.blueOuttake = parseVertex(fieldConfig["blueOuttake"]);
    sim.field.redOuttake = parseVertex(fieldConfig["redOuttake"]);
}



void ConfigReader::parseSimVehicleConfig(const YAML::Node& vehicleConfig)
{
    //
    // Load vehicle params
    //

    sim.vehicle.boundingPolygonFrontLeft = parsePolygon(vehicleConfig["boundingPolygonFrontLeft"]);
    sim.vehicle.boundingPolygonFrontRight = parsePolygon(vehicleConfig["boundingPolygonFrontRight"]);
    sim.vehicle.boundingPolygonRearLeft = parsePolygon(vehicleConfig["boundingPolygonRearLeft"]);
    sim.vehicle.boundingPolygonRearRight = parsePolygon(vehicleConfig["boundingPolygonRearRight"]);
    sim.vehicle.boundingPolygonBumperFrontLeft = parsePolygon(vehicleConfig["boundingPolygonBumperFrontLeft"]);
    sim.vehicle.boundingPolygonBumperFrontRight = parsePolygon(vehicleConfig["boundingPolygonBumperFrontRight"]);
    sim.vehicle.boundingPolygonBumperLeft = parsePolygon(vehicleConfig["boundingPolygonBumperLeft"]);
    sim.vehicle.boundingPolygonBumperRight = parsePolygon(vehicleConfig["boundingPolygonBumperRight"]);
    sim.vehicle.boundingPolygonBumperRearLeft = parsePolygon(vehicleConfig["boundingPolygonBumperRearLeft"]);
    sim.vehicle.boundingPolygonBumperRearRight = parsePolygon(vehicleConfig["boundingPolygonBumperRearRight"]);
    sim.vehicle.ingestibleRegionCenter = parsePolygon(vehicleConfig["ingestibleRegionCenter"]);
    sim.vehicle.ingestibleRegionLeft = parsePolygon(vehicleConfig["ingestibleRegionLeft"]);
    sim.vehicle.ingestibleRegionRight = parsePolygon(vehicleConfig["ingestibleRegionRight"]);
    sim.vehicle.tubeRegion = parsePolygon(vehicleConfig["tubeRegion"]);
    sim.vehicle.mass = vehicleConfig["weight"].as<float>() * LBS_TO_KG;

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
        sim.vehicle.drivetrain.leftMotorMaxSpeed = drivetrainConfig["leftMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        sim.vehicle.drivetrain.rightMotorMaxSpeed = drivetrainConfig["rightMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
    }

    //
    // Load vehicle drivetrain params
    //

    YAML::Node intakeConfig = vehicleConfig["intake"];

    if (intakeConfig)
    {
        sim.vehicle.intake.centerMotorMaxSpeed = intakeConfig["centerMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        sim.vehicle.intake.leftMotorMaxSpeed = intakeConfig["leftMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        sim.vehicle.intake.rightMotorMaxSpeed = intakeConfig["rightMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
        sim.vehicle.intake.tubeMotorMaxSpeed = intakeConfig["tubeMotorMaxSpeed"].as<float>() * RPM_TO_RADS_PER_SEC;
    }

    //
    // Load vehicle LIDAR params
    //

    YAML::Node lidarConfig = vehicleConfig["lidar"];

    if (lidarConfig)
    {
        sim.vehicle.lidar.minRange = lidarConfig["minRange"].as<float>();
        sim.vehicle.lidar.maxRange = lidarConfig["maxRange"].as<float>();
        sim.vehicle.lidar.laserFrequency = lidarConfig["laserFrequency"].as<float>();
        sim.vehicle.lidar.motorFrequency = lidarConfig["motorFrequency"].as<float>();
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



void ConfigReader::parseSimViewConfig(const YAML::Node& simViewConfig)
{
    simView.ip = simViewConfig["ip"].as<std::string>();
    simView.port = simViewConfig["port"].as<int>();
}



std::vector<Geometry::Vertex2d> ConfigReader::parsePolygon(const YAML::Node& node)
{
    std::vector<Geometry::Vertex2d> result;
    for (const auto& vertex : node)
    {
        result.emplace_back(parseVertex(vertex));
    }
    return result;
}



Geometry::Vertex2d ConfigReader::parseVertex(const YAML::Node& node)
{
    Geometry::Vertex2d v{};
    v.x = node["x"].as<float>() * IN_TO_M;
    v.y = node["y"].as<float>() * IN_TO_M;
    return v;
}
