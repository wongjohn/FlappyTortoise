#pragma once
#include "cocos2d.h"

class MainGameLayer : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MainGameLayer);

	void setPhysicsWorld(cocos2d::PhysicsWorld* physicsWorld) {_world = physicsWorld;};
	void update(float dt);

	static cocos2d::ParticleSystemQuad* createPaopaoAt(cocos2d::Point position);
private:
	cocos2d::PhysicsWorld* _world;

	void initBackground();
	std::vector<cocos2d::Sprite*> seas;

	void initScore();
	float _time;
	cocos2d::Label* _scoreLabel;

	void initPaopao();

	cocos2d::Size visibleSize;
	cocos2d::Vec2 visibleOrigin;
	float delta;
};
