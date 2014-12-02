#include "MainMenuScene.h"
#include "MainGameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

cocos2d::Scene* MainMenuLayer::createScene() {
	auto scene = Scene::create();
	auto layer = MainMenuLayer::create();
	scene->addChild(layer);
	return scene;
}//end createScene

bool MainMenuLayer::init() {
	if(!Layer::init()) return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Background image
	auto background = Sprite::create("MainMenu/ui_background_normal-hd.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Vec2::ZERO);
	this->addChild(background);

	//SpriteFrame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/UI_GameStartMenuLayer-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/UI_GameMenuText_cn-hd.plist");

	//The start button
	auto startGameBtn = Sprite::createWithSpriteFrameName("ui_button_box02_02.png");
	auto startGameBtnPush = Sprite::createWithSpriteFrameName("ui_button_box02_01.png");
	auto startGameFont = Sprite::createWithSpriteFrameName("ui_2p_010.png");

	auto startGameMenuItem = MenuItemSprite::create(startGameBtn, startGameBtnPush, CC_CALLBACK_1(MainMenuLayer::goToMainGameScene, this));
	auto startGameMenu = Menu::createWithItem(startGameMenuItem);
	startGameMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 50);
	startGameFont->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 60);

	this->addChild(startGameMenu, 1);
	this->addChild(startGameFont, 1);

	//Paopao
	this->addChild(MainGameLayer::createPaopaoAt(Point::ZERO));
	this->addChild(MainGameLayer::createPaopaoAt(Point(visibleSize.width, 0)));

	//Tortoise
	tortoise = Tortoise::create();
	this->addChild(tortoise);
	tortoise->setPosition(100, visibleSize.height/3);
	
	moveBy = MoveBy::create(3.f, Vec2(visibleSize.width - 200,0));
	moveBy->retain();
	moveBy->setTag(1);
	auto seq = Sequence::create(moveBy, CallFunc::create(CC_CALLBACK_0(MainMenuLayer::reachCallback, this)),nullptr);
	seq->setTag(2);
	tortoise->runAction(seq);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music.mp3", true);
	return true;
}//end init
void MainMenuLayer::onExit() {
	Layer::onExit();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void MainMenuLayer::goToMainGameScene(cocos2d::Ref* pSender) {
	Director::getInstance()->replaceScene(MainGameLayer::createScene());
}//end goToMainGameScene

void MainMenuLayer::reachCallback() {
	tortoise->stopActionByTag(2);
	auto reverse = moveBy->reverse();
	reverse->retain();
	moveBy->release();
	moveBy = reverse;
	auto rotate = RotateBy::create(1.f, Vec3(0, 180, 0));
	auto seq = Sequence::create(rotate, moveBy, CallFunc::create(CC_CALLBACK_0(MainMenuLayer::reachCallback,this)), nullptr);
	seq->setTag(2);
	tortoise->runAction(seq);
}
