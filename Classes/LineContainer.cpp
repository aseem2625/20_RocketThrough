//
//  LineContainer.cpp
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/04/30.
//
//

#include "LineContainer.h"

USING_NS_CC;
using namespace DrawPrimitives;

LineContainer::LineContainer()
{
    // member variables
    _dash = 10;
    _dashSpace = 10;
    _lineType = LINE_NONE;
    _screenSize = Director::getInstance()->getVisibleSize();
    _energyLineX = _screenSize.width * .98;
    _energyHeight = _screenSize.height * .8;
    
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
    if ( lineContainer && lineContainer->DrawNode::init() ) {
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
    this->clear();
    
    // draw line
    switch (_lineType) {
        case LINE_NONE:
            break;
            
        case LINE_TEMP:
            drawSegment(_tip, _pivot, 8, Color4F::WHITE);
            drawDot(_pivot, 10, Color4F::WHITE);
            break;
            
        case LINE_DASHED:
            drawDot(_pivot, 10, Color4F::WHITE);
            
            int segments = _lineLentgh / (_dash + _dashSpace);
            float t = 0;
            float x_, y_;
            for (int i = 0; i < segments + 1; i++) {
                x_ = _pivot.x + t * (_tip.x - _pivot.x);
                y_ = _pivot.y + t * (_tip.y - _pivot.y);
                
                drawDot(Point(x_, y_), 4, Color4F::WHITE);
                
                t += (float)1 / segments;
            }
            break;
    }
    
    // draw engergy bar
    drawSegment(Point(_energyLineX, _screenSize.height * .1), Point(_energyLineX, _screenSize.height * .9), 8, Color4F::BLACK);
    drawSegment(Point(_energyLineX, _screenSize.height * .1), Point(_energyLineX, _screenSize.height * .1 + _energy * _energyHeight), 8, Color4F::ORANGE);
    
    this->DrawNode::draw(renderer, transform, transformUpdated);
}

void LineContainer::setEnergyDecrement(float energyDecrement)
{
    _energyDecrement += energyDecrement;
    if (_energyDecrement > .07) _energyDecrement = .07;
}

