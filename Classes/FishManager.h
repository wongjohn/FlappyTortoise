#pragma once
#include "cocos2d.h"

class Fish : public cocos2d::Sprite {
public:
	void initFish();

	virtual cocos2d::Animate* playSwimAnimation() = 0;
	virtual cocos2d::Animate* playDeathAnimation() = 0;
	
	virtual void updateFishMovement(float dt) = 0;
	
	cocos2d::Animation* initAnimationWithSpriteFrameName(std::string spriteName);
	void update(float dt);
private:
	cocos2d::Size _visibleSize;
};

class MarlinFish : public Fish {
public:
	bool init();
	CREATE_FUNC(MarlinFish);

	cocos2d::Animate* playSwimAnimation();
	cocos2d::Animate* playDeathAnimation();
	void updateFishMovement(float dt);
private:
	cocos2d::Animation* _marlinSwim;
	cocos2d::Animation* _marlinDeath;
};

class GrouperFish : public Fish {
public:
	bool init();
	CREATE_FUNC(GrouperFish);

	cocos2d::Animate* playSwimAnimation();
	cocos2d::Animate* playDeathAnimation();
	void updateFishMovement(float dt);
private:
	cocos2d::Animation* _grouperSwim;
	cocos2d::Animation* _grouperDeath;
};

class FishManager : public cocos2d::Ref {
public :
	static FishManager* getInstance();
	Fish* createFishWithType(int type);

private:
	bool init();
	FishManager();
	static FishManager* _fishManager;
	
};

