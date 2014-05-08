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



class LineContainer : public cocos2d::DrawNode
{
public:
    LineContainer();
    static LineContainer *create();
    
    virtual void update(float delta);
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
    
    void setEnergyDecrement(float energyDecrement);
    void reset(void);
    
    CC_SYNTHESIZE(float, _energy, Energy);
    CC_SYNTHESIZE(Point, _pivot, Pivot);
	CC_SYNTHESIZE(Point, _tip, Tip);
    CC_SYNTHESIZE(float, _lineLentgh, LineLength);
    CC_SYNTHESIZE(LineType, _lineType, LineType);
    
protected:
    
    
private:
    
    int _dash;
    int _dashSpace;
    Size _screenSize;
    float _energyLineX;
    float _energyHeight;
    
    float _energyDecrement;
    
    
};
#endif /* defined(___0_RocketThrough__LineContainer__) */
