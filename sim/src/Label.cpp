/**
 * Copyright (c) 2020 Team 3260
 */

#include "Label.h"


Label::Label(const std::string& text, float pos, float fontSize) :
        _desiredPos(pos)
{
    this->setText(text);
    this->setCharacterSize(fontSize);
    this->setPosition(osg::Vec3(_leftPadding, pos, 0));
    this->setFont("/data/fonts/helvetica.ttf");
    this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    this->setDataVariance(osg::Object::DYNAMIC);
}
