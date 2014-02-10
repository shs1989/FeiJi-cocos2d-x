#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
class BulletLayer : public CCLayer
{
public:
	BulletLayer(void);
	~BulletLayer(void);
	CREATE_FUNC(BulletLayer);
	virtual bool init();
	void addBullet(float dt);
	void bulletMoveFinished(CCNode* pSender);
	void removeBullet(CCSprite* bullet);
	void startShoot(float delay = 0.0f);
	void stopShoot();


public:
	CCArray* m_pAllBullet;
	CCSpriteBatchNode* bulletBatchNode;
};

