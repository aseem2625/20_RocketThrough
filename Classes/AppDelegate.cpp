#include "AppDelegate.h"

#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // マルチディスプレイ対応
    Size screenSize = director->getWinSize();
    Size designSize = Size(768, 1024);
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);
    
    if (screenSize.width > 768) {
        FileUtils::getInstance()->setSearchPaths({"ipadhd"});
    } else if (screenSize.width > 320) {
        if (screenSize.height / screenSize.width >= 1.5) {
            FileUtils::getInstance()->setSearchPaths({"iphonehd"});
        } else {
            FileUtils::getInstance()->setSearchPaths({"ipad"});
        }
    } else {
        FileUtils::getInstance()->setSearchPaths({"iphone"});
    }
    director->setContentScaleFactor(designSize.height/screenSize.height);
    
    // BGMプリロード
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("background.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("pickup.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("bombRelease.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("rocket.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("shipBoom.wav");
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4f);
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
