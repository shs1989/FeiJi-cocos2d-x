#include "cocos2d.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "EnemyLayer.h"
#include "MutiBulletsLayer.h"
#include "UFOLayer.h"
#include "ControlLayer.h"
#include "SimpleAudioEngine.h"
const int ENEMY1_SCORE=1000;
const int ENEMY2_SCORE=6000;
const int ENEMY3_SCORE=30000;

const int MAX_BIGBOOM_COUNT=100000;
const int TAG_BIGBOOM_MENUITEM=1;
const int TAG_BIGBOOMCOUNT_LABEL=2;
USING_NS_CC;
class GameLayer : public CCLayer
{
public:
	GameLayer(void);
	~GameLayer(void);

	virtual bool init();

	static CCScene* scene();

	void backgroundMove(float dt);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void registerWithTouchDispatcher();

	void update(float dt);

	void menuBigBoomCallback(CCObject* pSender);

	void updateBigBoomItem(int bigBoomCount);
	static Level getCurLevel();
	GameLayer* _gameLayer;
	virtual void keyBackClicked();
private:
	CCSprite* background1;
	CCSprite* background2;
	PlaneLayer* planerLayer;
	BulletLayer* bulletLayer;
	EnemyLayer* enemyLayer;
	UFOLayer* ufoLayer;
	MutiBulletsLayer* mutiBulletsLayer;
	CCMenu* menuBigBoom;
	CCLabelBMFont* bigBoomCountItem;
	ControlLayer* controlLayer;
	CREATE_FUNC(GameLayer);

	int score;	
	int bigBoomCount;
	static Level level;
};

