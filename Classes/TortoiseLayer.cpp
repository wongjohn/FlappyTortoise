#include <iostream>
#include "TortoiseLayer.h"

USING_NS_CC;

bool TortoiseLayer::init() {
	if(!Layer::init()) return false;
	auto s = Director::getInstance()->getVisibleSize();

	_tortoise = Tortoise::create();
	_tortoise->initTortoisePhysicsBody();
	_tortoise->setPosition(Vec2(s.width / 3.f, s.height / 2.f));
	addChild(_tortoise);
	
	//����¼���������Ϊtortoise�Ĵ�������tortoise��onTouch
	auto eventListener = EventListenerTouchAllAtOnce::create();
	eventListener->onTouchesBegan = CC_CALLBACK_2(TortoiseLayer::onTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
	//���֧�ּ��̿ո��
	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = CC_CALLBACK_2(TortoiseLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	_fishManager = FishManager::getInstance();
	schedule(CC_SCHEDULE_SELECTOR(TortoiseLayer::spawFishes), 0.8f);
	return true;
}//end method init

void TortoiseLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) {
	if(_tortoise) {
		_tortoise->onTouch();
	}
}//end onTouchesBegan

void TortoiseLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	if(_tortoise && keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
		_tortoise->onTouch();
	}
}

void TortoiseLayer::spawFishes(float dt) {
	auto fish = _fishManager->createFishWithType(cocos2d::random(0, 1));
	fish->runAction(RepeatForever::create(fish->playSwimAnimation()));
	this->addChild(fish);
}

