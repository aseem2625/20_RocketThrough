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