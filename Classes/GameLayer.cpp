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
    this->addChild(background, kBackground);
    
    // Line Container
    _lineContainer = LineContainer::create();
    this->addChild(_lineContainer);
    
    // 画像シートをキャッシュに登録
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png", 100);
    this->addChild(_gameBatchNode, kForeground);
    
    // ロケット
    _rocket = Rocket::create();
    _rocket->setPosition(_screenSize.width/2, _screenSize.height * .1);
    this->addChild(_rocket, kForeground, kSpriteRocket);
    
    // 惑星
    GameSprite *planet;
    std::ostringstream spriteFramename;
    float planetPosition[7][2] = {{.25, .8}, {.8, .45}, {.75, .8}, {.5, .5}, {.18, .45}, {.8, .15}, {.18, .1}};
    for (int i = 1; i <= 7; i++) {
        spriteFramename.str("");
        spriteFramename << "planet_" << i << ".png";
        planet = GameSprite::createWithSpriteFrameName(spriteFramename.str());
        planet->setPosition(Point(_screenSize.width * planetPosition[i][0],
                                  _screenSize.height * planetPosition[i][1]));
        _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
        _planets.pushBack(planet);
    }
    
    // 「SCORE:」ラベル
    Sprite *scoreLabel = Sprite::createWithSpriteFrameName("label_score.png");
    scoreLabel->setPosition(_screenSize.width * .4, _screenSize.height * .95);
    _gameBatchNode->addChild(scoreLabel, kBackground);
    
    // 得点の表示
    _scoreDisplay = Label::createWithBMFont("font.fnt", "0");
    _scoreDisplay->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    _scoreDisplay->setPosition(_screenSize.width * .48, _screenSize.height * .95);
    this->addChild(_scoreDisplay, kBackground);
    
    // ポーズボタン
    _pauseBtn = GameSprite::createWithSpriteFrameName("btn_pause_off.png");
    _pauseBtn->setVisible(false);
    _pauseBtn->setPosition(_screenSize.width * .06, _screenSize.height * .95);
    _gameBatchNode->addChild(_pauseBtn, kBackground);
    
    // タイトルロゴ
    _intro = CCSprite::createWithSpriteFrameName("logo.png");
    _intro->setPosition(_screenSize.width * .5, _screenSize.height * .55);
    Sprite *play = CCSprite::createWithSpriteFrameName("label_play.png");
    play->setPosition(_intro->boundingBox().size.width * .5, -_intro->boundingBox().size.height * .5);
    _intro->addChild(play);
    _gameBatchNode->addChild(_intro, kForeground);
    
    // ゲームオーバーロゴ
    _gameOver = Sprite::createWithSpriteFrameName("gameOver.png");
    _gameOver->setPosition(_screenSize.width * .5, _screenSize.height * .55);
    _gameOver->setVisible(false);
    _gameBatchNode->addChild(_gameOver, kForeground);
    
    // ポーズロゴ
    _paused = Sprite::createWithSpriteFrameName("label_paused.png");
    _paused->setPosition(_screenSize.width * .5, _screenSize.height * .55);
    _paused->setVisible(false);
    _gameBatchNode->addChild(_paused, kForeground);
}

void GameLayer::createParticles(void)
{
    
}

void GameLayer::createStarGrid(void)
{
    //create grid
    float gridFrame = _screenSize.width * 0.1f;
    int tile = 32;
    int rows = (_screenSize.height - 4 * gridFrame)/tile;
    int cols = (_screenSize.width  - 2 * gridFrame)/tile;
    
    long count = _planets.size();
    GameSprite * planet;
    Point cell;
    bool overlaps;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cell = Point(gridFrame + c * tile, 2 * gridFrame + r * tile);
            overlaps = false;
            for (int j = 0; j < count; j++) {
                planet = (GameSprite *) _planets.at(j);
                if (pow(planet->getPositionX() - cell.x, 2) + pow(planet->getPositionY() - cell.y, 2) <= pow(planet->getRadius() * 1.2f, 2)) {
                    overlaps = true;
                }
            }
            if (!overlaps) _grid.push_back(cell);
        }
    }
    CCLOG("POSSIBLE STARS: %lu", _grid.size());
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
    if (_state == kGameIntro) {
        
        _intro->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        //resetGame();
        return;
        
    } else if (_state == kGameOver) {
        
        _gameOver->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        //resetGame();
        return;
        
    } else if (_state == kGamePaused) {
        
        _pauseBtn->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_pause_off.png"));
        _paused->setVisible(false);
        _state = kGamePlay;
        _running = true;
        return;
    }
    
    if (!_playing) return;
    
    
}
