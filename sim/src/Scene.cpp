/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include <osgViewer/Viewer>
#include <osgGA/TerrainManipulator>
#include <osg/ShapeDrawable>
#include "Color.h"
#include "Scene.h"


Scene::Scene()
{
    _root = new osg::Group;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->addChild(geode);
    osg::Matrix mat;
    mat.makeRotate(osg::DegreesToRadians(-90.0), osg::Y_AXIS, osg::DegreesToRadians(90.0), osg::Z_AXIS, 0, osg::X_AXIS);
    pat->setAttitude(mat.getRotate());

    // Render the belt assembly
    {
        // Bottom sprocket
        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, 0), _beltRadius, _beltWidth);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
        shape->setColor(Color::Orange);
        geode->addDrawable(shape);
    }

    {
        // Top sprocket
        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(_beltLength, 0, 0), _beltRadius, _beltWidth);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
        shape->setColor(Color::Orange);
        geode->addDrawable(shape);
    }

    {
        // Front and back of belt
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

        // Front vertices
        vertices->push_back(osg::Vec3(0, -_beltRadius, -_beltWidth/2));
        vertices->push_back(osg::Vec3(0, -_beltRadius, _beltWidth/2));
        vertices->push_back(osg::Vec3(_beltLength, -_beltRadius, _beltWidth/2));
        vertices->push_back(osg::Vec3(_beltLength, -_beltRadius, -_beltWidth/2));

        // Back vertices
        vertices->push_back(osg::Vec3(0, _beltRadius, -_beltWidth/2));
        vertices->push_back(osg::Vec3(0, _beltRadius, _beltWidth/2));
        vertices->push_back(osg::Vec3(_beltLength, _beltRadius, _beltWidth/2));
        vertices->push_back(osg::Vec3(_beltLength, _beltRadius, -_beltWidth/2));

        colors->push_back(Color(Color::Orange, 127));
        geom->setVertexArray(vertices);
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
        geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); // Turn off lighting
        geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); // Turn on blending
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertices->size())); // Geometry type is TRIANGLES
        geode->addDrawable(geom);
    }

    {
        // Motor shaft
        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength/2), _motorShaftRadius, _motorShaftLength);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
        shape->setColor(Color::Gray);
        geode->addDrawable(shape);
    }

    {
        // Motor
        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength + _motorLength/2), _motorRadius, _motorLength);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
        shape->setColor(Color::Gray);
        geode->addDrawable(shape);
    }

    {
        // Encoder
        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, _beltWidth/2 + _motorShaftLength - _encoderLength/2), _encoderRadius, _encoderLength);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
        shape->setColor(Color::Green);
        geode->addDrawable(shape);
    }

    {
        // Elevator carriage
        _carriagePat = new osg::PositionAttitudeTransform;
        pat->addChild(_carriagePat);

        osg::ref_ptr<osg::Geode> carriageGeode = new osg::Geode;
        _carriagePat->addChild(carriageGeode);

        osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0, -_beltRadius - _carriageLengthY/2, 0), _carriageLengthX, _carriageLengthY, _carriageLengthZ);
        osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
        shape->setColor(Color::Blue);
        carriageGeode->addDrawable(shape);
    }

    _root->addChild(pat);
}



void Scene::render()
{
    osgViewer::Viewer viewer;
    viewer.setSceneData(_root);
    osg::ref_ptr<osgGA::TerrainManipulator> manip = new osgGA::TerrainManipulator;
    manip->setAllowThrow(false);
    viewer.setCameraManipulator(manip);
    viewer.getCamera()->setClearColor(Color::Black);
    viewer.run();
}



void Scene::setElevatorPosition(float pos)
{
    _carriagePat->setPosition(osg::Vec3(pos, 0, 0));
}
