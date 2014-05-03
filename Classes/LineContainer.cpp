//
//  LineContainer.cpp
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/04/30.
//
//

#include "LineContainer.h"

USING_NS_CC;

LineContainer::LineContainer()
{
    // member variables
    _dash = 10;
    _dashSpace = 10;
    _lineType = LINE_NONE;
    _screenSize = Director::getInstance()->getVisibleSize();
    _energyLineX = _screenSize.width * .98;
    _energyLineHeight = _screenSize.height * .8;
    
    // GL 描画する線の長さを設定
    glLineWidth(8 * CC_CONTENT_SCALE_FACTOR());
    
    // リセット
    this->reset();
}

void LineContainer::reset()
{
    _energy = 1.0;
    _energyDecrement = .015;
}

LineContainer *LineContainer::create()
{
    LineContainer *lineContainer = new LineContainer();
    if (lineContainer) {
        lineContainer->autorelease();
        return lineContainer;
    }
    CC_SAFE_DELETE(lineContainer);
    return NULL;
}

void LineContainer::update(float delta)
{
    _energy -= _energyDecrement * delta;
    if (_energy < 0) _energy = 0;
}

void LineContainer::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    
}

void LineContainer::setEnergyDecrement(float energyDecrement)
{
    _energyDecrement += energyDecrement;
    if (_energyDecrement > .07) _energyDecrement = .70;
}

