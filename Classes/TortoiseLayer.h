#pragma once
#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "Math/Vec2.h"
#include "FishManager.h"
#include "Tortoise.h"

class TortoiseLayer : public cocos2d::Layer {
public:
	bool init();
	CREATE_FUNC(TortoiseLayer);

private :
	//作为tortoise的代理，调用tortoise的onTouch
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	Tortoise* _tortoise;
	void spawFishes(float dt);
	FishManager* _fishManager;
};
