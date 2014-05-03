//
//  GameSprite.cpp
//  20_RocketThrough
//
//  Created by Shin Takano on 2014/05/03.
//
//

#include "GameSprite.h"

GameSprite* GameSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    GameSprite *gameSprite = new GameSprite();
    if (gameSprite && gameSprite->initWithSpriteFrameName(spriteFrameName)) {
        gameSprite->setRadius(gameSprite->getBoundingBox().size.width/2);
        gameSprite->autorelease();
        return gameSprite;
    }
    CC_SAFE_DELETE(gameSprite);
    return NULL;
}