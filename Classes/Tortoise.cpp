#include "Tortoise.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

bool Tortoise::init() {
	if(!Node::init()) return false;
	//3Dģ�Ͷ���
	initTortoiseAnimation();

	//����Ч������
	initUVAnimation();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sfx_die.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sfx_wing.ogg");
	return true;
}//end init
void Tortoise::initTortoisePhysicsBody() {
	//�����������PhysicsBody
	auto boundingBox = _sprite->getBoundingBox();
	auto physicsBody = PhysicsBody::createBox(boundingBox.size, PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(-100, -20));
	//physicsBody->setCategoryBitmask(0x01);
	//physicsBody->setCollisionBitmask(0x02);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	_sprite->setPhysicsBody(physicsBody);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Tortoise::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

}//end initTortoisePhysicsBody

bool Tortoise::onContactBegin(PhysicsContact& contact) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_DIE);
	_sprite->stopAllActions();
	_sprite->getPhysicsBody()->setGravityEnable(false);
	
	auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 120);
	gameOverLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height/2 + 10);
	this->getParent()->addChild(gameOverLabel);

	_sprite->runAction(_hurt);
	this->scheduleOnce([](float dt) {
		Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, MainMenuLayer::createScene()));
	}, 1.f, "step_key");
	return true;
}//end onContactBegin

//3Dģ�Ͷ���
void Tortoise::initTortoiseAnimation() {
	//����ģ��
	std::string fileName = "3D/tortoise.c3b";
	_sprite = Sprite3D::create(fileName);
	_sprite->setRotation3D(Vec3(0, 180, 0));

	_sprite->setScale(0.2f);
	auto s = Director::getInstance()->getVisibleSize();
	
	addChild(_sprite);

	//��ȡ����������Ϣ
	_animation3D = Animation3D::create(fileName);
	//����ʼ��1.933���ȡΪ��Ӿ����
	_swim = Animate3D::create(_animation3D, 0.f, 1.933f);
	_swim->retain();
	//��1.933�뵽2.8���ȡΪ���˵Ķ���
	_hurt = Animate3D::create(_animation3D, 1.933f, 2.8f);
	_hurt->retain();
	auto pSequence = Sequence::create(_swim, nullptr);
	_sprite->runAction(RepeatForever::create(pSequence));
}//end initTortoiseAnimation

//����Ч������
void Tortoise::initUVAnimation() {
	// ��vsh��fshװ���һ��������Shader�ļ���
	auto glprogram = GLProgram::createWithFilenames("3D/UVAnimation.vsh", "3D/UVAnimation.fsh");
	// ��Shader�ļ��������Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// �������������õ�Shader
	_sprite->setGLProgramState(glprogramstate);

	//�����������õ���ͼ��
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage("3D/tortoise.png");
	//����ͼ���ø�Shader�еı���ֵu_texture1
	glprogramstate->setUniformTexture("u_texture1", textrue1);

	//����������ͼ��
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage("3D/caustics.png");
	//����ͼ���ø�Shader�еı���ֵu_lightTexture
	glprogramstate->setUniformTexture("u_lightTexture", textrue2);

	//ע�⣬���ڲ�����ͼ������ϣ�����ڽ���UV����ʱ�ܲ����ķ�����Ч��������������������UVѰַ��ʽΪGL_REPEAT��
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	textrue2->setTexParameters(tRepeatParams);

	//�������������һ���������ɫ����������Ϊ��ɫ��
	Vec4  tLightColor(1.0,1.0,1.0,1.0);
	glprogramstate->setUniformVec4("v_LightColor",tLightColor);

	//������һ�Σ���Ϊ�˽������Զ����Shader�����ǵ�ģ�Ͷ�����֯��ʽ����ƥ�䡣ģ�͵Ķ�������һ�����λ�ã����ߣ�ɫ�ʣ������Լ���������Ϣ����Shader��Ҫ���ڲ���Ӧ�Ķ�������ͨ����ģ����Ӧ�Ķ����������ݽ��а󶨲�����ȷ��ʾ�����㡣
	long offset = 0;
	auto attributeCount = _sprite->getMesh()->getMeshVertexAttribCount();
	for (auto k = 0; k < attributeCount; k++) {
		auto meshattribute = _sprite->getMesh()->getMeshVertexAttribute(k);
		glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
			meshattribute.size,
			meshattribute.type,
			GL_FALSE,
			_sprite->getMesh()->getVertexSizeInBytes(),
			(GLvoid*)offset);
		offset += meshattribute.attribSizeBytes;
	}

	//uv������ʼֵ��Ϊ0
	_lightAni.x = _lightAni.y = 0;
}//end initUVAnimation

void Tortoise::attackTortoise() {
	_sprite->stopAllActions();
	_sprite->runAction(_hurt);
}//end attackTortoise

void Tortoise::continueSwim() {
	_sprite->stopAllActions();
	_sprite->runAction(_swim);
}//end continueSwim

//���»��ƺ���(����Ч��)
void Tortoise::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	auto glprogramstate = _sprite->getGLProgramState();
	if(glprogramstate)
	{
		_lightAni.x += 0.01;
		if(_lightAni.x > 1.0)
		{
			_lightAni.x-= 1.0;
		}
		_lightAni.y += 0.01;
		if(_lightAni.y > 1.0)
		{
			_lightAni.y-= 1.0;
		}
		glprogramstate->setUniformVec2("v_animLight",_lightAni);
	}
	Node::draw(renderer,transform,flags);
}

void Tortoise::onTouch() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_WING);
	this->_sprite->getPhysicsBody()->setVelocity(Vect(0, 300));
}

