//
//  LineContainer.h
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/04/30.
//
//

#ifndef ___0_RocketThrough__LineContainer__
#define ___0_RocketThrough__LineContainer__

#include "cocos2d.h"

USING_NS_CC;

typedef enum LineType: int {
    LINE_NONE,
    LINE_TEMP,
    LINE_DASHED,
} LineType;



class LineContainer : public cocos2d::Node
{
public:
    LineContainer();
    static LineContainer *create();
    
    virtual void update(float delta);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    void setEnergyDecrement(float energyDecrement);
    
protected:
    
    
private:
    void reset(void);
    
    int _dash;
    int _dashSpace;
    LineType _lineType;
    Size _screenSize;
    float _energyLineX;
    float _energyLineHeight;
    
    float _energy;
    float _energyDecrement;
    
    
};
#endif /* defined(___0_RocketThrough__LineContainer__) */
