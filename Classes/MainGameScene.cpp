#include <iostream>
#include "MainGameScene.h"
#include "TortoiseLayer.h"

USING_NS_CC;

Scene* MainGameLayer::createScene() {
	auto scene = Scene::createWithPhysics();
	
	auto layer = MainGameLayer::create();
	auto physicsWorld = scene->getPhysicsWorld();
	//Debug Physics World
	//physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	physicsWorld->setGravity(Vect(0, -900));
	layer->setPhysicsWorld(physicsWorld);
	scene->addChild(layer);
	
	//create border
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto node = Node::create();
	auto physicsBody = PhysicsBody::createEdgeBox(visibleSize,PhysicsMaterial(0.1f, 1, 0.0f));
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	node->setPhysicsBody(physicsBody);
	node->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	scene->addChild(node);

	//Debug AABB
	layer->addChild(DrawNode::create());

	return scene;
}//end createScene

bool MainGameLayer::init() {
	if(!Layer::init()) return false;
	
	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	visibleOrigin = director->getVisibleOrigin();

	//添加游戏背景
	this->initBackground();

	//添加乌龟层
	this->addChild(TortoiseLayer::create());

	//添加得分标签层
	this->initScore();

	//气泡
	this->initPaopao();
	return true;
}//end init

void MainGameLayer::initBackground() {
	auto background = Node::create();
	delta = 55.f;
	auto sea = Sprite::create("3D/Sea.jpg");
	sea->setAnchorPoint(Vec2::ZERO);
	sea->setPosition(visibleOrigin);
	background->addChild(sea);
	seas.push_back(sea);

	sea = Sprite::create("3D/Sea.jpg");
	sea->setAnchorPoint(Vec2::ZERO);
	sea->setFlippedX(true);
	sea->setPosition(visibleOrigin.x + visibleSize.width + delta, 0);
	background->addChild(sea);
	seas.push_back(sea);

	this->addChild(background);

	scheduleUpdate();
}//end initBackground

void MainGameLayer::update(float dt) {
	for(auto sea : seas) {
		sea->setPosition(sea->getPositionX() - 10, sea->getPositionY());
		//超出了界线，重新设置到屏幕右边
		if(sea->getPositionX() < (-1 * visibleSize.width - delta )) {
			sea->setPosition(visibleOrigin.x + visibleSize.width + delta, 0);
		}//end if

	}//end for loop

	_time += dt;
	char string[15];
	sprintf(string, "Time: %2.2f", _time);
	_scoreLabel->setString(string);
}//end update

void MainGameLayer::initScore() {
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	_time = 0;
	_scoreLabel = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.plist");
	_scoreLabel->setAnchorPoint(Vec2(0, 1));
	_scoreLabel->setPosition(visibleOrigin.x + 20, visibleSize.height - 10);

	this->addChild(_scoreLabel);
}//end initScore

void MainGameLayer::initPaopao() {
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
	this->addChild(this->createPaopaoAt(visibleOrigin));
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->addChild(this->createPaopaoAt(Point(visibleSize.width, 0)));
}//end initPaopao

ParticleSystemQuad* MainGameLayer::createPaopaoAt(Point position){

	//Create the paraticle of bubble
	auto paopao = ParticleSystemQuad::create("lizi_qipao.plist");

	//Set the bubble position type form the ground
	paopao->setPositionType(ParticleSystemQuad::PositionType::RELATIVE);

	paopao->setPosition(position);
	paopao->setScale(2.0f);

	return paopao;
}//end createPaopao
