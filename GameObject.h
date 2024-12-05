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
		worldMatrix = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(6, 1, 6), 0, 0, 0);
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
	std::vector<Matrix> worldMatrices;
	Vec3 position;
	int num = 10;
	Foliage(DXcore* core, std::string gemName, std::string texName1, std::string texName2, std::string texName3, Vec3 _position, Vec3 scale, int _num) {
		shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader.txt", core);
		model.init(gemName, core);
		position = _position;
		num = _num;
		textures.load(core, texName1);
		textures.load(core, texName2);
		textures.load(core, texName3);
		for (int i = 0; i < num; i++) {
			int seedX = rand() % 70;
			int seedZ = rand() % 90;
			Vec3 v = position + Vec3(seedX, 0, seedZ);
			worldMatrices.push_back(Matrix::worldMatrix(v, scale, 0, 0, 0));
		}
	}

	void draw(DXcore* core, Matrix& vp) {
		for (int i = 0; i < num; i++) {
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrices[i]);
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

			shader.apply(core);
			model.draw(core, &textures, &shader);
		}
	}
};

class NPC: public GameObject {
public:
	Vec3 position;
	float health;
	float aggroRange;
	float attackRange;
	bool insideAggroRange;
	bool insideAttackRange;

	Model_animated model;
	AnimationInstance animationInstance;
	AnimationController animController;

	NPC(){}
	NPC(DXcore* core) {
		health = 100.f;
		aggroRange = 30.f;
		attackRange = 16.f;
		insideAggroRange = false;
		insideAttackRange = false;
		position = Vec3(0, 0, 0);

		shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);
		model.init("Models/TRex.gem", core);
		worldMatrix = Matrix::worldMatrix(position, Vec3(2, 2, 2), 0, 0, 0);
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

class Player : public NPC {
public:
	Player(DXcore* core) {
		position = Vec3(0, 0, 20);

		shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);
		model.init("Models/Soldier1.gem", core); 

		worldMatrix = Matrix::worldMatrix(position, Vec3(0.06, 0.06, 0.06), 0,0, 0);
		// initialize animation instance
		animationInstance.animation = &model.animation;
		animationInstance.currentAnimation = "idle";
		// load texture
		textures.load(core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png");
		textures.load(core, "Textures/MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");
	}

	void updatePos(Vec3 camPos) {
		position = camPos - Vec3(-0.8,9.5,-1.3);
		worldMatrix = Matrix::worldMatrix(position, Vec3(0.058, 0.058, 0.058), 0, 0, 0);
	}

	void updateAnimationInstance(float dt) {
		// update current animation state 
		//animController.updateNPCState(insideAggroRange, insideAttackRange, health, dt);
		//animationInstance.update(animController.stateToString(), dt);
		animationInstance.update("rifle aiming idle", dt);
	}
};