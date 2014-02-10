#include "cocos2d.h"
USING_NS_CC;

class GameOverLayer :
	public CCLayer
{
public:
	GameOverLayer(void);

	~GameOverLayer(void);

	virtual bool init();

	static GameOverLayer* create(int passScore);

	void menuBackCallback(CCObject* pSender);

	
	void beginChangeHighestScore(CCNode* pNode);

	void showAD();

public:

	int score;

	static int highestHistoryScore;

	CCLabelBMFont* highestScore;
};
