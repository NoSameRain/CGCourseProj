#pragma once
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "AnimationController.h"

class GameObject {
public:
	Matrix worldMatrix;
	Shader shader;
	TextureManager textures;
	
	GameObject(){}
};

class Ground :public GameObject {
public:
	Plane plane;
	Ground(DXcore* core) {
		shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader_ground.txt", core);
		plane.init(core);
		worldMatrix = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(10, 10, 10), 0, 0, 0);
	}

	void draw(DXcore* core, Matrix& vp) {
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader.apply(core);
		plane.mesh.draw(core);
	}
};

class Foliage : public GameObject {
public:	
	Model_static model;
	Foliage(DXcore* core) {
		shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader.txt", core);
		model.init("Models/pine.gem", core);
		worldMatrix = Matrix::worldMatrix(Vec3(-4, 0, -4), Vec3(0.05, 0.05, 0.05), 0, 0, 0);

		textures.load(core, "Textures/bark09.png");
		textures.load(core, "Textures/pine branch.png");
		textures.load(core, "Textures/stump01.png");
	}

	void draw(DXcore* core, Matrix& vp) {
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader.apply(core);
		model.draw(core, &textures, &shader);
	}
};

class NPC: public GameObject {
public:
	Vec3 position;
	float health;
	float speed;
	float aggroRange;
	float attackRange;
	bool insideAggroRange;
	bool insideAttackRange;

	Model_animated model;
	AnimationInstance animationInstance;
	AnimationController animController;

	NPC(DXcore* core) {
		health = 100.f;
		aggroRange = 20.f;
		attackRange = 10.f;
		insideAggroRange = false;
		insideAttackRange = false;
		position = Vec3(0, 0, 0);

		shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);
		model.init("Models/TRex.gem", core);
		worldMatrix = Matrix::worldMatrix(position, Vec3(1, 1, 1), 0, 0, 0);
		// initialize animation instance
		animationInstance.animation = &model.animation;
		animationInstance.currentAnimation = "Idle";
		// load texture
		textures.load(core, "Textures/T-rex_Base_Color.png");
	}

	void checkInsideAggroRange(Vec3 playerPos) {
		if (position.distance(playerPos) <= aggroRange) insideAggroRange = true;
		else insideAggroRange = false;
	}

	void checkInsideAttackRange(Vec3 playerPos) {
		if (position.distance(playerPos) <= attackRange) insideAttackRange = true;
		else insideAttackRange = false;
	}

	void updateAnimationInstance(float dt, Vec3 playerPos) {
		checkInsideAggroRange(playerPos);
		checkInsideAttackRange(playerPos);
		// update current animation state 
		animController.updateNPCState(insideAggroRange, insideAttackRange, health, dt);
		animationInstance.update(animController.stateToString(), dt);
	}

	void draw(DXcore* core, Matrix& vp) {
		shader.updateConstantVS("Animated", "staticMeshBuffer", "bones", animationInstance.matrices);
		shader.updateConstantVS("Animated", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);

		shader.apply(core);
		model.draw(core, &textures, &shader);
	}

};