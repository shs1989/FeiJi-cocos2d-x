#include "cocos2d.h"
USING_NS_CC;
class WelcomeLayer : public CCLayer
{
public:
	WelcomeLayer(void);
	~WelcomeLayer(void);
	virtual bool init();
	static CCScene* scene();
	void initViews();
	void preLoadMusic();
	void loadingDown(CCNode* pNode);
	bool isHaveSaveFile();
	void getHighestHistorySorce();
	virtual void keyBackClicked();
	CREATE_FUNC(WelcomeLayer);
};

