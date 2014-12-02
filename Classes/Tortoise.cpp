#include "Tortoise.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

bool Tortoise::init() {
	if(!Node::init()) return false;
	//3D模型动画
	initTortoiseAnimation();

	//波光效果动画
	initUVAnimation();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sfx_die.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sfx_wing.ogg");
	return true;
}//end init
void Tortoise::initTortoisePhysicsBody() {
	//添加物理属性PhysicsBody
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

//3D模型动画
void Tortoise::initTortoiseAnimation() {
	//加载模型
	std::string fileName = "3D/tortoise.c3b";
	_sprite = Sprite3D::create(fileName);
	_sprite->setRotation3D(Vec3(0, 180, 0));

	_sprite->setScale(0.2f);
	auto s = Director::getInstance()->getVisibleSize();
	
	addChild(_sprite);

	//获取骨骼动画信息
	_animation3D = Animation3D::create(fileName);
	//从起始到1.933秒截取为游泳动作
	_swim = Animate3D::create(_animation3D, 0.f, 1.933f);
	_swim->retain();
	//从1.933秒到2.8秒截取为受伤的动作
	_hurt = Animate3D::create(_animation3D, 1.933f, 2.8f);
	_hurt->retain();
	auto pSequence = Sequence::create(_swim, nullptr);
	_sprite->runAction(RepeatForever::create(pSequence));
}//end initTortoiseAnimation

//波光效果动画
void Tortoise::initUVAnimation() {
	// 将vsh与fsh装配成一个完整的Shader文件。
	auto glprogram = GLProgram::createWithFilenames("3D/UVAnimation.vsh", "3D/UVAnimation.fsh");
	// 由Shader文件创建这个Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// 给精灵设置所用的Shader
	_sprite->setGLProgramState(glprogramstate);

	//创建海龟所用的贴图。
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage("3D/tortoise.png");
	//将贴图设置给Shader中的变量值u_texture1
	glprogramstate->setUniformTexture("u_texture1", textrue1);

	//创建波光贴图。
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage("3D/caustics.png");
	//将贴图设置给Shader中的变量值u_lightTexture
	glprogramstate->setUniformTexture("u_lightTexture", textrue2);

	//注意，对于波光贴图，我们希望它在进行UV动画时能产生四方连续效果，必须设置它的纹理UV寻址方式为GL_REPEAT。
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	textrue2->setTexParameters(tRepeatParams);

	//在这里，我们设置一个波光的颜色，这里设置为白色。
	Vec4  tLightColor(1.0,1.0,1.0,1.0);
	glprogramstate->setUniformVec4("v_LightColor",tLightColor);

	//下面这一段，是为了将我们自定义的Shader与我们的模型顶点组织方式进行匹配。模型的顶点数据一般包括位置，法线，色彩，纹理，以及骨骼绑定信息。而Shader需要将内部相应的顶点属性通道与模型相应的顶点属性数据进行绑定才能正确显示出顶点。
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

	//uv滚动初始值设为0
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

//重新绘制函数(波光效果)
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

