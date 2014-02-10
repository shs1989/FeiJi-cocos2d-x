#include "WelcomeLayer.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"
#include "GameOverLayer.h"

WelcomeLayer::WelcomeLayer(void)
{
}


WelcomeLayer::~WelcomeLayer(void)
{
}


bool WelcomeLayer::init(){
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		initViews();
		
		getHighestHistorySorce();
		this->setKeypadEnabled(true);
		bRet = true;

	} while (0);

	return bRet;


}


CCScene* WelcomeLayer::scene(){
	CCScene* sc = NULL;
	do 
	{
		sc = CCScene::create();
		CC_BREAK_IF(!sc);
		WelcomeLayer* layer = WelcomeLayer::create();
		CC_BREAK_IF(!layer);
		sc->addChild(layer);
		return sc;
	} while (0);

	return sc;



}

void WelcomeLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}
void WelcomeLayer::initViews(){

	preLoadMusic();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot_background.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui/shoot.plist");
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	CCSprite* background=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
	background->setPosition(ccp(winSize.width/2,winSize.height/2));
	this->addChild(background);


	CCSprite* copyright=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shoot_copyright.png"));
	copyright->setAnchorPoint(ccp(0.5,0));
	copyright->setPosition(ccp(winSize.width/2,winSize.height/2));
	this->addChild(copyright);

	CCSprite* loading=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading1.png"));
	loading->setPosition(ccp(winSize.width/2,winSize.height/2-40));
	this->addChild(loading);

	

	

	CCAnimation* animation=CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading1.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading2.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading3.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("game_loading4.png"));


	CCAnimate* animate=CCAnimate::create(animation);
	CCRepeat* repeat = CCRepeat::create(animate,2);

	CCCallFuncN* repatDown = CCCallFuncN::create(this,callfuncN_selector(WelcomeLayer::loadingDown));
	CCSequence* sequence = CCSequence::create(repeat,repatDown,NULL);
	loading->runAction(sequence);

}
void WelcomeLayer::loadingDown(CCNode* pNode){
	CCLog("loading");
	
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,GameLayer::scene()));
}

void WelcomeLayer::preLoadMusic(){

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/game_music.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/bullet.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy1_down.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy2_down.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy3_down.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/game_over.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/get_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/get_double_laser.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/use_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/big_spaceship_flying.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/achievement.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/out_porp.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/button.mp3");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.mp3",true);
}

bool WelcomeLayer::isHaveSaveFile()
{
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);
		CCUserDefault::sharedUserDefault()->flush();
		return false;
	}
	else
	{
		return true;
	}
}

void WelcomeLayer::getHighestHistorySorce()
{
	if (isHaveSaveFile())
	{
		GameOverLayer::highestHistoryScore=CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore",0);
	}
}