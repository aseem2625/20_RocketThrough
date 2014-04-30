#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum GameState : int {
    kGameIntro,
} GameState;

typedef enum zPosition : int {
    kBackGround,
} GameState;

class GameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameLayer);
    virtual bool init();
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    
    void update(float dt);
    
private:
    void createGameScreen(void);
    void createParticles(void);
    void createStarGrid(void);
    
    Size _screenSize;
    bool _drawing;
    float _minimumLineLength;
    GameState _state;
};

#endif // __HELLOWORLD_SCENE_H__
