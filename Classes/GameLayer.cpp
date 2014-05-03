#include "GameLayer.h"

USING_NS_CC;

Scene* GameLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    // member variables
    _screenSize = Director::getInstance()->getVisibleSize();
    _drawing = false;
    _minimumLineLength = _screenSize.width * .07;
    _state = kGameIntro;
    
    // prepare sprites
    createGameScreen();
    createParticles();
    createStarGrid();
    
    // enable touch
    EventListenerTouchAllAtOnce *touches = EventListenerTouchAllAtOnce::create();
    touches->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    touches->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
    touches->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches, this);
    
    // update
    this->scheduleUpdate();
    
    return true;
}

void GameLayer::createGameScreen(void)
{
    // 背景
    Sprite *background = Sprite::create("bg.png");
    background->setPosition(_screenSize.width/2, _screenSize.height/2);
    this->addChild(background, kBackGround);
    
    
}

void GameLayer::createParticles(void)
{
    
}

void GameLayer::createStarGrid(void)
{
    
}

void GameLayer::update(float dt)
{
    
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    
}
