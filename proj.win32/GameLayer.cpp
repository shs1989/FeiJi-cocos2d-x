#include "GameLayer.h"
#include "Enemy.h"

Level GameLayer::level=EASY;
GameLayer::GameLayer(void)
{
	 background1 = NULL;
	 background2 = NULL;
	 planerLayer = NULL;
	 bulletLayer = NULL;
	 enemyLayer = NULL;
	 mutiBulletsLayer = NULL;
	 ufoLayer = NULL;
	 controlLayer = NULL;
	 score = 0;
	 bigBoomCount=0;
}


GameLayer::~GameLayer(void)
{
}



bool GameLayer::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		level=HARD;

		//加载background1
		background1=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background1->setAnchorPoint(ccp(0,0));
		background1->setPosition(ccp(0,0));
		this->addChild(background1);

		//加载background2
		background2=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background2->setAnchorPoint(ccp(0,0));
		background2->setPosition(ccp(0,background2->getContentSize().height-2));
		this->addChild(background2);

		planerLayer = PlaneLayer::create();
		this->addChild(planerLayer);

		bulletLayer = BulletLayer::create();
		this->addChild(bulletLayer);
		bulletLayer->startShoot();

		enemyLayer = EnemyLayer::create();
		this->addChild(enemyLayer);


		this->mutiBulletsLayer=MutiBulletsLayer::create();
		this->addChild(mutiBulletsLayer);

		this->ufoLayer=UFOLayer::create();
		this->addChild(ufoLayer);

		this->controlLayer = ControlLayer::create();
		this->addChild(controlLayer);
		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		this->setTouchEnabled(true);

		this->scheduleUpdate();
		this->setKeypadEnabled(true);
		bRet = true;

	} while (0);
	return bRet;
}
void GameLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}
CCScene* GameLayer::scene(){
	CCScene* sc = NULL;

	do 
	{
		sc = CCScene::create();
		CC_BREAK_IF(!sc);
		GameLayer* layer = GameLayer::create();
		CC_BREAK_IF(!layer);
		sc->addChild(layer);

	} while (0);

	return sc;

}

void GameLayer::backgroundMove(float dt){

	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY(background1->getPositionY()+background1->getContentSize().height-2);
	if (background2->getPositionY() == 0)
	{
		background1->setPositionY(0);

	}
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

	return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	
	if (this->planerLayer->isAlive)
	{
		CCPoint beginPoint = pTouch->getLocationInView();
		beginPoint = CCDirector::sharedDirector()->convertToGL(beginPoint);
		
		CCRect planRect = this->planerLayer->getChildByTag(AIRPLANE)->boundingBox();

		planRect.origin.x -= 15;
		planRect.origin.y -= 15;

		planRect.size.width += 30;
		planRect.size.height += 30;
		//判断触摸点是否在飞机范围内

		
		if(planRect.containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch))) 
		{ 
			CCPoint endPoint = pTouch->getPreviousLocationInView();
			endPoint = CCDirector::sharedDirector()->convertToGL(endPoint);

			CCPoint offSet = ccpSub(beginPoint,endPoint);
			CCPoint toPoint = ccpAdd(this->planerLayer->getChildByTag(AIRPLANE)->getPosition(),offSet);
			this->planerLayer->MoveTo(toPoint);
		}

	}

}

void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

void GameLayer::registerWithTouchDispatcher(){
	CCDirector *pDirector=CCDirector::sharedDirector();  
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0,true);  
}

void GameLayer::update(float dt){
	//创建一个CCArray，用以存放待删除的子弹，也就是此帧中被检测到碰撞的子弹   
	CCArray* bulletsToDelete = CCArray::create();
	bulletsToDelete->retain();
	CCObject* bt,*et,*ut;
	//enemy1

	CCARRAY_FOREACH(bulletLayer->m_pAllBullet,bt){ //遍历所有子弹

		CCSprite* bullet = (CCSprite*)bt;
		//创建一个CCArray，用以存放待删除的敌机，也就是此子弹击中的敌机  
		CCArray* enemy1ToDelete = CCArray::create();
		enemy1ToDelete->retain();
		//遍历所有敌机
		CCARRAY_FOREACH(enemyLayer->m_pAllEnemy1,et){
			Enemy* enemy1 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1ToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
			}

		}

		CCARRAY_FOREACH(enemy1ToDelete,et){
			Enemy* enemy1 = (Enemy*)et;
			enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1ToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt){
		CCSprite* bullet = (CCSprite*)bt;
		bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();



	//enemy2

	CCARRAY_FOREACH(bulletLayer->m_pAllBullet,bt){ //遍历所有子弹

		CCSprite* bullet = (CCSprite*)bt;
		//创建一个CCArray，用以存放待删除的敌机，也就是此子弹击中的敌机  
		CCArray* enemy2ToDelete = CCArray::create();
		enemy2ToDelete->retain();
		//遍历所有敌机
		CCARRAY_FOREACH(enemyLayer->m_pAllEnemy2,et){
			Enemy* enemy2 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()  > 1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);

				}else if (enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2ToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
			}

		}

		CCARRAY_FOREACH(enemy2ToDelete,et){
			Enemy* enemy2 = (Enemy*)et;
			enemyLayer->enemy1Blowup(enemy2);
		}
		enemy2ToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt){
		CCSprite* bullet = (CCSprite*)bt;
		bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();


	//enemy3

	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				//如果life>1,移除bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				//如果life==1,移除enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					
					score+=ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->removeBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();


	CCArray* mutiBulletsToDelete=CCArray::create();
	mutiBulletsToDelete->retain();
	CCObject* mbt;

	////enemy1 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();


	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy3 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				//如果life>1,移除bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				//如果life==1,移除enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
 					this->controlLayer->updateScore(score);
				}
				//此时处在animate阶段,不做处理
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();
	mutiBulletsToDelete->release();


	CCRect airplaneRect=this->planerLayer->getChildByTag(AIRPLANE)->boundingBox();
	airplaneRect.origin.x+=30;
	airplaneRect.size.width-=60;

	//enemy1 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
	{
		Enemy* enemy1=(Enemy*)et;
		if (enemy1->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->stopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planerLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy2 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
	{
		Enemy* enemy2=(Enemy*)et;
		if (enemy2->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy2->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->stopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planerLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy3 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
	{
		Enemy* enemy3=(Enemy*)et;
		if (enemy3->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy3->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->stopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planerLayer->Blowup(score);
				return;
			}
		}
	}
	
	//mutiBullets & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllMutiBullets,ut)
	{
		CCSprite* mutiBullets=(CCSprite*)ut;
		if (this->planerLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(mutiBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/get_double_laser.mp3");
			this->ufoLayer->RemoveMutiBullets(mutiBullets);
			this->bulletLayer->stopShoot();
			this->mutiBulletsLayer->StartShoot();
			this->bulletLayer->startShoot(6.2f);
		}
	}

	//bigBoom & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllBigBoom,ut)
	{
		CCSprite* bigBoom=(CCSprite*)ut;
		if (this->planerLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(bigBoom->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/get_bomb.mp3");
			this->ufoLayer->RemoveBigBoom(bigBoom);
			bigBoomCount++;
			updateBigBoomItem(bigBoomCount);
		}
	}
}

void GameLayer::menuBigBoomCallback(CCObject* pSender){
	if(bigBoomCount>0 && !CCDirector::sharedDirector()->isPaused())
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/use_bomb.mp3");
		bigBoomCount--;
		score+=this->enemyLayer->m_pAllEnemy1->count()*ENEMY1_SCORE;
		score+=this->enemyLayer->m_pAllEnemy2->count()*ENEMY2_SCORE;
		score+=this->enemyLayer->m_pAllEnemy3->count()*ENEMY3_SCORE;
		this->enemyLayer->removeAllEnemy();
		updateBigBoomItem(bigBoomCount);
		this->controlLayer->updateScore(score);
	}
}

void GameLayer::updateBigBoomItem(int bigBoomCount){

	CCSprite* normalBomb=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bomb.png"));
	CCSprite* pressedBomb=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bomb.png"));
	if (bigBoomCount<0)
	{
		return;
	}
	else if (bigBoomCount==0)
	{
		if(this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM,true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}
	else if (bigBoomCount==1)
	{
		//加入bigBoomItemMenu
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			CCMenuItemImage* pBigBoomItem=CCMenuItemImage::create();
			pBigBoomItem->initWithNormalSprite(normalBomb,pressedBomb,NULL,this,menu_selector(GameLayer::menuBigBoomCallback));
			pBigBoomItem->setPosition(ccp(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			menuBigBoom=CCMenu::create(pBigBoomItem,NULL);
			menuBigBoom->setPosition(CCPointZero);
			this->addChild(menuBigBoom,0,TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}
	else
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			CCMenuItemImage* pBigBoomItem=CCMenuItemImage::create();
			pBigBoomItem->initWithNormalSprite(normalBomb,pressedBomb,NULL,this,menu_selector(GameLayer::menuBigBoomCallback));
			pBigBoomItem->setPosition(ccp(normalBomb->getContentSize().width/2+10,normalBomb->getContentSize().height/2+10));
			menuBigBoom=CCMenu::create(pBigBoomItem,NULL);
			menuBigBoom->setPosition(CCPointZero);
			this->addChild(menuBigBoom,0,TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
		if (bigBoomCount>=0 && bigBoomCount<=MAX_BIGBOOM_COUNT)
		{
			CCString* strScore=CCString::createWithFormat("X%d",bigBoomCount);
			bigBoomCountItem=CCLabelBMFont::create(strScore->m_sString.c_str(),"font/font.fnt");
			bigBoomCountItem->setColor(ccc3(143,146,147));
			bigBoomCountItem->setAnchorPoint(ccp(0,0.5));
			bigBoomCountItem->setPosition(ccp(normalBomb->getContentSize().width+15,normalBomb->getContentSize().height/2+5));
			this->addChild(bigBoomCountItem,0,TAG_BIGBOOMCOUNT_LABEL);
		}
	}
}
Level GameLayer::getCurLevel()
{
	return level;
}