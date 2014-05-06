#include "GameLayer.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

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
    this->addChild(_lineContainer, kBackground);
    
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
    for (int i = 0; i < 7; i++) {
        spriteFramename.str("");
        spriteFramename << "planet_" << i + 1 << ".png";
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
    // ロケットのジェット噴射
    _jet = ParticleSystemQuad::create("jet.plist");
    _jet->setSourcePosition(Point(-_rocket->getRadius() * .8, 0));
    _jet->setAngle(180);
    _jet->stopSystem();
    this->addChild(_jet, kBackground);
    
    _boom = ParticleSystemQuad::create("boom.plist");
    _boom->stopSystem();
    this->addChild(_boom, kForeground);
    
    _comet = ParticleSystemQuad::create("comet.plist");
    _comet->stopSystem();
    _comet->setPosition(0, _screenSize.height * .6);
    _comet->setVisible(false);
    this->addChild(_comet, kForeground);
    
    _pickup = ParticleSystemQuad::create("plink.plist");
    _pickup->stopSystem();
    this->addChild(_pickup, kMiddleground);
    
    _warp = ParticleSystemQuad::create("warp.plist");
    _warp->setPosition(_rocket->getPosition());
    this->addChild(_warp, kBackground);
    
    _star = ParticleSystemQuad::create("star.plist");
    _star->stopSystem();
    _star->setVisible(false);
    this->addChild(_star, kBackground, kSpriteStar);
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
    if (!_playing) return;
    
    if (_lineContainer->getLineType() != LINE_NONE) {
        _lineContainer->setTip(_rocket->getPosition());
    }
    
    // track collision with sides
    if (_rocket->isCollidedWithSides()) {
        _lineContainer->setLineType(LINE_NONE);
    }
    
    _rocket->update(dt);
    
    // update jet particle so ti follows rocket
    if (!_jet->isActive()) _jet->resetSystem();
    _jet->setRotation(_rocket->getRotation());
    _jet->setPosition(_rocket->getPosition());
    
    // update _comet
    _cometTimer += dt;
    float newY;
    
    if (_cometTimer > _cometInterval) {
        _cometTimer = 0;
        if (!_comet->isVisible()) {
            _comet->setPositionX(0);
            newY = _screenSize.height * .2 +  (float)rand() / ((float)RAND_MAX / _screenSize.height * .6);
            if (newY > _screenSize.height * .9) {
                newY = _screenSize.height * .9;
            }
            _comet->setPositionY(newY);
            _comet->setVisible(true);
            _comet->resetSystem();
        }
    }
    
    if (_comet->isVisible()) {
        // collision with commet
        if (_rocket->getPosition().getDistance(_comet->getPosition()) < _rocket->getRadius()) {
            if (_rocket->isVisible()) this->killPlayer();
        }
        _comet->setPositionX(_comet->getPositionX() + 50 * dt);
        if (_comet->getPositionX() > _screenSize.width * 1.5) {
            _comet->stopSystem();
            _comet->setVisible(false);
        }
    }
}

void GameLayer::resetGame () {
    
    _rocket->setPosition(_screenSize.width * 0.5f, _screenSize.height * 0.1f);
    _rocket->setOpacity(255);
    _rocket->setVisible(true);
    _rocket->reset();
    
    _cometInterval = 4;
    _cometTimer = 0;
    _timeBetweenPickups = 0.0;
    
    _score = 0;
    char szValue[100] = {0};
    sprintf(szValue, "%i", _score);
    _scoreDisplay->setString(szValue);
    
    _lineContainer->reset();
    
    //shuffle grid cells
    std::random_shuffle(_grid.begin(), _grid.end());
    _gridIndex = 0;
    
    resetStar();
    
    _warp->stopSystem();
    
    _running = true;
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect("rocket.wav", true);
}

void GameLayer::resetStar()
{
    
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    if (!_playing) return;
    
    Touch *touch = touches.at(0);
    if (!touch) return;
    
    Point tap = touch->getLocation();
    
    // track if tapping on ship
    if (_rocket->getPosition().getDistance(tap) < _rocket->getRadius() * 1.2) {
        // clear lines
        _lineContainer->setLineLength(LINE_NONE);
        _rocket->setRotationOrientation(RotationOrientation::NONE);
        _drawing = true;
    }
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    if (!_playing) return;
    if (!_drawing) return;
    
    Touch *touch = touches.at(0);
    if (!touch) return;
    
    Point tap = touch->getLocation();
    if (_rocket->getPosition().getDistance(tap) > _minimumLineLength) {
        _rocket->select(true);
        _lineContainer->setPivot(tap);
        _lineContainer->setLineType(LINE_TEMP);
    } else {
        _rocket->select(false);
        _lineContainer->setLineType(LINE_NONE);
    }
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    if (_state == kGameIntro) {
        
        _intro->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        resetGame();
        return;
        
    } else if (_state == kGameOver) {
        
        _gameOver->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        resetGame();
        return;
        
    } else if (_state == kGamePaused) {
        
        _pauseBtn->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_pause_off.png"));
        _paused->setVisible(false);
        _state = kGamePlay;
        _running = true;
        return;
    }
    
    if (!_playing) return;
    Touch *touch = touches.at(0);
    if (!touch) return;
    Point tap = touch->getLocation();
    
    if (_pauseBtn->getBoundingBox().containsPoint(tap)) {
        _paused->setVisible(true);
        _state = kGamePaused;
        _pauseBtn->setSpriteFrame("btn_pause_on.png");
        _playing = false;
        return;
    }
    
    _drawing = false;
    _rocket->select(false);
    
    // if we are showing a temp line
    if (_lineContainer->getLineType() == LINE_TEMP) {
        _lineContainer->setPivot(tap);
        _lineContainer->setLineLength(_rocket->getPosition().getDistance(tap));
        // set up rocket
        _rocket->setPivot(tap);
        
        float circle_length = _lineContainer->getLineLength() * 2 * M_PI;
        int iterations = floor(circle_length / _rocket->getSpeed());
        _rocket->setAngularSpeed(2 * M_PI / iterations);
        
        Point clockwise = (_rocket->getPosition() + _rocket->getPivot()).getRPerp();
        float dot = clockwise.dot(_rocket->getVector());
        if (dot > 0) {
            _rocket->setAngularSpeed(_rocket->getAngularSpeed() * -1);
            _rocket->setRotationOrientation(RotationOrientation::CLOCKWIZE);
            _rocket->setTargetRotation(CC_RADIANS_TO_DEGREES(atan2(clockwise.y, clockwise.x)));
        } else {
            _rocket->setRotationOrientation(RotationOrientation::COUNTER_CLOCKWIZE);
            _rocket->setTargetRotation(CC_RADIANS_TO_DEGREES(atan2(-clockwise.y, -clockwise.x)));
        }
        _lineContainer->setLineType(LINE_DASHED);
    }
}
