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

	//�����ڹ�
	void attackTortoise();
	
	//�����ζ�
	void continueSwim();

	//���»��ƺ���(����Ч������)
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);

	void onTouch();

	void initTortoisePhysicsBody();
protected:
	//�ڹ�
	cocos2d::Sprite3D* _sprite;
	//��������
	cocos2d::Animation3D* _animation3D;
	//��ǰ�ζ�
	cocos2d::Animate3D* _swim;
	//���˶���
	cocos2d::Animate3D* _hurt;

	//uv����ֵ
	cocos2d::Vec2 _lightAni;

private:
	//3Dģ�Ͷ����͸������Գ�ʼ��
	void initTortoiseAnimation();
	//����Ч������
	void initUVAnimation();

	bool onContactBegin(cocos2d::PhysicsContact& contact);
};
