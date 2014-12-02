#pragma once
#include "cocos2d.h"
// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SOUND_DIE        "sounds/sound_die.ogg"
#else
#define SOUND_DIE        "sounds/sound_die.mp3"
#endif // CC_PLATFOR_ANDROID

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SOUND_WING        "sounds/sound_wing.ogg"
#else
#define SOUND_WING        "sounds/sound_wing.mp3"
#endif // CC_PLATFOR_ANDROID


class Tortoise : public cocos2d::Node {
public :
	bool init();
	CREATE_FUNC(Tortoise);

	//击中乌龟
	void attackTortoise();
	
	//继续游动
	void continueSwim();

	//重新绘制函数(波光效果动画)
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);

	void onTouch();

	void initTortoisePhysicsBody();
protected:
	//乌龟
	cocos2d::Sprite3D* _sprite;
	//骨骼动画
	cocos2d::Animation3D* _animation3D;
	//向前游动
	cocos2d::Animate3D* _swim;
	//受伤动作
	cocos2d::Animate3D* _hurt;

	//uv滚动值
	cocos2d::Vec2 _lightAni;

private:
	//3D模型动画和刚体属性初始化
	void initTortoiseAnimation();
	//波光效果动画
	void initUVAnimation();

	bool onContactBegin(cocos2d::PhysicsContact& contact);
};
