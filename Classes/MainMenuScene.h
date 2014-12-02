#pragma once
#include "cocos2d.h"
#include "Tortoise.h"

class MainMenuLayer : cocos2d::Layer {
public:
	bool init();
	CREATE_FUNC(MainMenuLayer);
	static cocos2d::Scene* createScene();

	void goToMainGameScene(cocos2d::Ref* pSender);
	void onExit() override;
private:
	void reachCallback();
	Tortoise* tortoise;
	cocos2d::MoveBy* moveBy;
};
