//
//  GameSprite.h
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/05/03.
//
//

#ifndef ___0_RocketThrough__GameSprite__
#define ___0_RocketThrough__GameSprite__

#include "cocos2d.h"

USING_NS_CC;

class GameSprite : public cocos2d::Sprite
{
public:
    static GameSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    CC_SYNTHESIZE(float, _radius, Radius);
    
protected:
    
    
private:
    
    
};
#endif /* defined(___0_RocketThrough__GameSprite__) */
