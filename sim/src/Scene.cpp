/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include "Color.h"
#include "Scene.h"


Scene::Scene(const ConfigReader& config) :
        _beltRadius(config.vehicle.constants.elevator.belt.radius),
        _beltWidth(config.vehicle.constants.elevator.belt.width),
        _beltLength(config.vehicle.constants.elevator.belt.length),
        _motorShaftRadius(config.vehicle.constants.elevator.motorShaft.radius),
        _motorShaftLength(config.vehicle.constants.elevator.motorShaft.length),
        _motorRadius(config.vehicle.constants.elevator.motor.radius),
        _motorLength(config.vehicle.constants.elevator.motor.length),
        _encoderRadius(config.vehicle.constants.elevator.encoder.radius),
        _encoderLength(config.vehicle.constants.elevator.encoder.length),
        _carriageLengthX(config.vehicle.constants.elevator.carriage.lengthX),
        _carriageLengthY(config.vehicle.constants.elevator.carriage.lengthY),
        _carriageLengthZ(config.vehicle.constants.elevator.carriage.lengthZ)
{
    _root = new osg::Group;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(geode);
    osg::Matrix mat;
    mat.makeRotate(osg::DegreesToRadians(-90.0), osg::Y_AXIS, osg::DegreesToRadians(90.0), osg::Z_AXIS, 0, osg::X_AXIS);
    pat->setAttitude(mat.getRotate());

    // Render the belt assembly

    // Bottom sprocket
    osg::ref_ptr<osg::ShapeDrawable> bottomSprocket = makeCylinder(osg::Vec3(0, 0, 0), _beltRadius, _beltWidth, Color::Orange);
    geode->addDrawable(bottomSprocket);

    // Top sprocket
    osg::ref_ptr<osg::ShapeDrawable> topSprocket = makeCylinder(osg::Vec3(_beltLength, 0, 0), _beltRadius, _beltWidth, Color::Orange);
    geode->addDrawable(topSprocket);

    // Front and back of belt
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0, -_beltRadius, -_beltWidth/2)); // Front vertices
    vertices->push_back(osg::Vec3(0, -_beltRadius, _beltWidth/2));
    vertices->push_back(osg::Vec3(_beltLength, -_beltRadius, _beltWidth/2));
    vertices->push_back(osg::Vec3(_beltLength, -_beltRadius, -_beltWidth/2));
    vertices->push_back(osg::Vec3(0, _beltRadius, -_beltWidth/2)); // Back vertices
    vertices->push_back(osg::Vec3(0, _beltRadius, _beltWidth/2));
    vertices->push_back(osg::Vec3(_beltLength, _beltRadius, _beltWidth/2));
    vertices->push_back(osg::Vec3(_beltLength, _beltRadius, -_beltWidth/2));
    osg::ref_ptr<osg::Geometry> belt = makeQuads(vertices, Color(Color::Orange, 127.0));
    geode->addDrawable(belt);

    // Motor shaft
    auto motorShaft = makeCylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength/2), _motorShaftRadius, _motorShaftLength, Color::Gray);
    geode->addDrawable(motorShaft);

    // Motor
    auto motor = makeCylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength + _motorLength/2), _motorRadius, _motorLength, Color::Gray);
    geode->addDrawable(motor);

    // Encoder
    auto encoder = makeCylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength - _encoderLength/2), _encoderRadius, _encoderLength, Color::Green);
    geode->addDrawable(encoder);

    // Elevator carriage
    _carriagePat = new osg::PositionAttitudeTransform;
    pat->addChild(_carriagePat);
    osg::ref_ptr<osg::Geode> carriageGeode = new osg::Geode;
    _carriagePat->addChild(carriageGeode);
    auto elevatorCarriage = makeBox(osg::Vec3(0, -_beltRadius - _carriageLengthY/2, 0), _carriageLengthX, _carriageLengthY, _carriageLengthZ, Color::Blue);
    carriageGeode->addDrawable(elevatorCarriage);

    _root->addChild(pat);
}



void Scene::update(const RobotModel& robot)
{
    // Update elevator position
    double elevatorPos = robot._state.elevatorCarriagePos;
    _carriagePat->setPosition(osg::Vec3(elevatorPos, 0, 0));
}



osg::ref_ptr<osg::ShapeDrawable> Scene::makeCylinder(const osg::Vec3& pos, float radius, float height, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(pos, radius, height);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
    shape->setColor(color);
    return shape;
}



osg::ref_ptr<osg::ShapeDrawable> Scene::makeBox(const osg::Vec3& pos, float lengthX, float lengthY, float lengthZ, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Box> box = new osg::Box(pos, lengthX, lengthY, lengthZ);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
    shape->setColor(color);
    return shape;
}



osg::ref_ptr<osg::Geometry> Scene::makeQuads(osg::ref_ptr<osg::Vec3Array> vertices, const osg::Vec4& color)
{
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(color);

    geom->setVertexArray(vertices);
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // Turn off lighting
    geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); // Turn on blending
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size())); // Geometry type is QUADS
    return geom;
}
