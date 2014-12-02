#include "FishManager.h"

USING_NS_CC;
Animation* Fish::initAnimationWithSpriteFrameName(std::string spriteName) {
	auto spriteFrames = SpriteFrameCache::getInstance();
	
	char* tmpSpriteName = new char[60];
	const auto spriteNameTemplate = spriteName + "_%.3d.png";
	int count= 1;
	Vector<SpriteFrame*> vector;
	SpriteFrame* tmpSpriteFrame;
	do {
		sprintf(tmpSpriteName, spriteNameTemplate.c_str(), count++);
		tmpSpriteFrame = spriteFrames->getSpriteFrameByName(std::string(tmpSpriteName));
		if(tmpSpriteFrame) {
			vector.pushBack(tmpSpriteFrame);
		}
	} while(tmpSpriteFrame);

	delete[] tmpSpriteName;
	return Animation::createWithSpriteFrames(vector, 0.1f);
}
void Fish::initFish() {
	_visibleSize = Director::getInstance()->getVisibleSize();
	this->setPosition(_visibleSize.width + this->getContentSize().width, _visibleSize.height / 6 * (cocos2d::random(1, 5)));
	
	this->scheduleUpdate();
}
void Fish::update(float dt) {
	 this->updateFishMovement(dt);
	 if(this->getPositionX() < -this->getContentSize().width) {
		 this->getParent()->removeChild(this, true);
	 }
}
bool MarlinFish::init() {
	if(!Sprite::initWithSpriteFrameName("MarlinsFish_actor_001.png")) return false;
	
	this->initFish();

	_marlinSwim = AnimationCache::getInstance()->getAnimation("marlinSwim");
	if(!_marlinSwim) {
		_marlinSwim = initAnimationWithSpriteFrameName("MarlinsFish_actor");
		AnimationCache::getInstance()->addAnimation(_marlinSwim, "marlinSwim");
	}

	_marlinDeath = AnimationCache::getInstance()->getAnimation("marlinDeath");
	if(!_marlinDeath) {
		_marlinDeath = initAnimationWithSpriteFrameName("MarlinsFish_death");
		AnimationCache::getInstance()->addAnimation(_marlinDeath, "marlinDeath");
	}

	auto physicsBody = PhysicsBody::createBox(Size(this->getContentSize().width / 2, this->getContentSize().height / 3 - 30));
	physicsBody->setDynamic(false);
	//physicsBody->setCategoryBitmask(0x02);
	//physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);

	return true;
}

Animate* MarlinFish::playSwimAnimation() {
	return Animate::create(_marlinSwim);
}

Animate* MarlinFish::playDeathAnimation() {
	return Animate::create(_marlinDeath);
}

void MarlinFish::updateFishMovement(float dt) {
	this->setPositionX(this->getPositionX() - 10);
}

bool GrouperFish::init() {
	if(!Sprite::initWithSpriteFrameName("GrouperFish_actor_001.png")) return false;
	
	this->initFish();

	_grouperSwim = AnimationCache::getInstance()->getAnimation("grouperSwim");
	if(!_grouperSwim) {
		_grouperSwim = initAnimationWithSpriteFrameName("GrouperFish_actor");
		AnimationCache::getInstance()->addAnimation(_grouperSwim, "grouperSwim");
	}

	_grouperDeath = AnimationCache::getInstance()->getAnimation("grouperDeath");
	if(!_grouperDeath) {
		_grouperDeath = initAnimationWithSpriteFrameName("GrouperFish_death");
		AnimationCache::getInstance()->addAnimation(_grouperDeath, "grouperDeath");
	}

	auto physicsBody = PhysicsBody::createBox(this->getContentSize() - Size(0, 30));
	physicsBody->setDynamic(false);
	//physicsBody->setCategoryBitmask(0x02);
	//physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);

	return true;
}

Animate* GrouperFish::playSwimAnimation() {
	return Animate::create(_grouperSwim);
}

Animate* GrouperFish::playDeathAnimation() {
	return Animate::create(_grouperDeath);
}

void GrouperFish::updateFishMovement(float dt) {
	this->setPositionX(this->getPositionX() - 10);
}

FishManager* FishManager::_fishManager = nullptr;

bool FishManager::init() {
	Director::getInstance()->getTextureCache()->addImage("fishes/FishActor-Marlin-hd.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fishes/FishActor-Marlin-hd.plist");
	Director::getInstance()->getTextureCache()->addImage("fishes/FishActor-Small-hd.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fishes/FishActor-Small-hd.plist");



	return true;
}

FishManager::FishManager() {
}

FishManager* FishManager::getInstance() {
	if(!_fishManager) {
		_fishManager = new (std::nothrow) FishManager();
		_fishManager->init();
	}
	return _fishManager;
}

Fish* FishManager::createFishWithType(int type) {
	switch (type) {
	case 0:
		return MarlinFish::create();
		break;
	case 1:
		return GrouperFish::create();
		break;
	}
	return nullptr;
}
