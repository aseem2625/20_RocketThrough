#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "LineContainer.h"
#include "Rocket.h"

USING_NS_CC;

typedef enum GameState : int {
    kGameIntro,
    kGamePlay,
    kGamePaused,
    kGameOver,
} GameState;

typedef enum zPosition : int {
    kBackground,
    kMiddleground,
    kForeground,
} zPosition;

typedef enum SpriteKind : int {
    kSpriteRocket,
    kSpritePlanet,
    kSpriteBoost,
    kSpriteStar
} SpriteKind;

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
    
    std::vector<Point> _grid;
    
    GameSprite *_pauseBtn;
    Sprite * _intro;
    Sprite *_gameOver;
    Sprite *_paused;
    Label *_scoreDisplay;
    GameSprite *_pauseButton;
    
    SpriteBatchNode *_gameBatchNode;
    Rocket *_rocket;
    Vector<GameSprite *> _planets;
    
    LineContainer *_lineContainer;
    
    Size _screenSize;
    GameState _state;
    bool _playing;
    bool _drawing;
    float _minimumLineLength;
};

#endif // __HELLOWORLD_SCENE_H__
