#pragma once
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "AnimationController.h"

//should not be here
void writeStringToFile(ofstream& outfile, std::string filePath) {
	size_t length = filePath.size();
	outfile.write(reinterpret_cast<const char*>(&length), sizeof(size_t));
	outfile.write(filePath.c_str(), length);
}

void readStringfromFile(ifstream& infile, std::string filePath) {
	size_t length;
	infile.read(reinterpret_cast<char*>(&length), sizeof(length));
	filePath.resize(length);
	infile.read(&filePath[0], length);
}

class GameObject {
public:
	Matrix worldMatrix;
	Shader shader;
	TextureManager textures;

	Vec3 position;
	Vec3 scale;

	std::vector<std::string> diffusePath, normalPath;
	std::string gemPath;
	
	GameObject(){}

};

class Ground :public GameObject {
public:
	Plane plane;
	Ground(DXcore* core, const Vec3 _position, const Vec3 _scale) {
		position = _position;
		scale = _scale;
		shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader.txt", core);
		plane.init(core);
		worldMatrix = Matrix::worldMatrix(position, scale,Vec3(0,0,0) );
		diffusePath.push_back("Textures/Grass001_1K-PNG_Color.png"); 
		normalPath.push_back("Textures/Grass001_1K-PNG_NormalDX.png");
		textures.load(core, diffusePath[0]);
		textures.load(core, normalPath[0]);
	}

	void draw(DXcore* core, Matrix& vp) {
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		// bind texture to t0 and t1
		plane.bindTexture(core, &textures, &shader, diffusePath[0], normalPath[0]);
		shader.apply(core);
		plane.mesh.draw(core);
	}
	void saveDatatoFile(ofstream& outfile) {
		// position, scale
		outfile.write(reinterpret_cast<const char*>(&position), sizeof(Vec3));
		outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
		// texture: diffus, normal
		for (auto& path : diffusePath) {
			writeStringToFile(outfile, path);
		}
		for (auto& path : normalPath) {
			writeStringToFile(outfile, path);
		}
		// shader? plane?
	}

	void loadDataFromFile(DXcore* core, ifstream& infile) {
		// position, scale
		infile.read(reinterpret_cast<char*>(&position), sizeof(Vec3));
		infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
		// worldMatrix
		worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));
		// texture: diffus, normal
		for (auto& path : diffusePath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		for (auto& path : normalPath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		// shader? plane?
	}
};

class SkyBox :public GameObject {
public:	
	Sphere sphere;
	float timer = 0.f;
	SkyBox(DXcore* core, const Vec3 _position, const Vec3 _scale) {
		position = _position;
		scale = _scale;
		shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader_albedo.txt", core);
		sphere.init(core);
		worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, M_PI));
		textures.load(core, "Textures/kloppenheim_06_puresky.png");
	}

	void draw(DXcore* core, Matrix& vp) {
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		// bind texture to t0 and t1
		sphere.bindTexture(core, &textures, &shader, "Textures/kloppenheim_06_puresky.png");
		shader.apply(core);
		sphere.mesh.draw(core);
	}
};

class Foliage : public GameObject {
public:	
	Model_static model;
	std::vector<Vec3> positions;
	std::vector<Matrix> worldMatrices;
	int num;
	float timer = 0.f;
	Foliage(DXcore* core, std::string VertexSHaderName, std::string gemName, const std::vector<std::string>& texturePaths, Vec3 _position, Vec3 _scale, int _num) {
		gemPath = gemName;
		shader.init(VertexSHaderName, "Shaders/PixelShader.txt", core);
		model.init(gemPath, core);
		position = _position;
		scale = _scale;
		num = _num;
		
		for (const auto& texPath : texturePaths) {
			textures.load(core, texPath);
		}

		for (int i = 0; i < num; i++) {
			int seedX = rand() % 140;
			int seedZ = rand() % 120;
			Vec3 pos = position + Vec3(seedX, 0, seedZ);
			positions.push_back(pos);
			worldMatrices.push_back(Matrix::worldMatrix(pos, scale, Vec3(0,0,0)));
		}
	}

	void draw(DXcore* core, Matrix& vp, float dt) {
		timer += dt;
		for (int i = 0; i < num; i++) {
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrices[i]);
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
			shader.updateConstantVS("StaticModel", "staticMeshBuffer", "time", &timer);

			for (int meshIndex = 0; meshIndex < model.meshes.size(); meshIndex++) {
				// bind texture to t0 and t1
				model.bindTexture(core, &textures, &shader, meshIndex);
				shader.apply(core);
				model.meshes[meshIndex].draw(core);
			}
		}
	}

	void saveDatatoFile(ofstream& outfile) {
		// number of foliage
		outfile.write(reinterpret_cast<const char*>(&num), sizeof(int));
		// position, scale
		for (auto& pos : positions) {
			outfile.write(reinterpret_cast<const char*>(&pos), sizeof(Vec3));
		}
		outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
		// mesh
		//writeStringToFile(outfile, gemPath);
		// texture: diffus, normal
		for (auto& path : diffusePath) {
			writeStringToFile(outfile, path);
		}
		for (auto& path : normalPath) {
			writeStringToFile(outfile, path);
		}
		
		
	}

	void loadDataFromFile(DXcore* core, ifstream& infile) {
		// number of foliage
		infile.read(reinterpret_cast<char*>(&num), sizeof(int));
		// position, scale
		positions.clear();
		Vec3 pos;
		while (infile.read(reinterpret_cast<char*>(&pos), sizeof(Vec3))) {
			positions.push_back(pos);
		}
		infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
		// worldMatrix
		worldMatrices.clear();
		for (int i = 0; i < num; i++) {
			worldMatrices.push_back(Matrix::worldMatrix(positions[i], scale, Vec3(0, 0, 0)));
		}
		// mesh
		//readStringfromFile(infile, gemPath);
		//model.init(gemPath, core);
		// texture: diffus, normal
		for (auto& path : diffusePath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		for (auto& path : normalPath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		
	}

};

class NPC: public GameObject {
public:
	float aggroRange;
	float attackRange;
	bool insideAggroRange;
	bool insideAttackRange;
	float health;

	Model_animated model;
	AnimationInstance animationInstance;
	AnimationController animController;

	NPC(){}
	NPC(DXcore* core, const Vec3 _position, const Vec3 _scale) {
		position = _position;
		scale = _scale;

		health = 100.f;
		aggroRange = 30.f;
		attackRange = 16.f;
		insideAggroRange = false;
		insideAttackRange = false;
		gemPath = "Models/TRex.gem";

		shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);
		model.init(gemPath, core);
		worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0,0,0));
		// initialize animation instance
		animationInstance.animation = &model.animation;
		animationInstance.currentAnimation = "Idle";
		// load texture
		textures.load(core, "Textures/T-rex_Base_Color.png");
		textures.load(core, "Textures/T-rex_Normal_OpenGL.png");
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

		// bind texture to t0 and t1: t0 for diffuse, t1 for normal map
		model.bindTexture(core, &textures, &shader, 0);
		model.bindTexture(core, &textures, &shader, 1);

		shader.apply(core);
		model.draw(core);
	}
	void saveDatatoFile(ofstream& outfile) {
		// position, scale
		outfile.write(reinterpret_cast<const char*>(&position), sizeof(Vec3));
		outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
		// mesh
		//writeStringToFile(outfile, gemPath);

		// texture: diffus, normal
		for (auto& path : diffusePath) {
			writeStringToFile(outfile, path);
		}
		for (auto& path : normalPath) {
			writeStringToFile(outfile, path);
		}
		// bool
		outfile.write(reinterpret_cast<const char*>(&insideAggroRange), sizeof(bool));
		outfile.write(reinterpret_cast<const char*>(&insideAttackRange), sizeof(bool));
		// mesh
		writeStringToFile(outfile, gemPath);
	}

	void loadDataFromFile(DXcore* core, ifstream& infile) {
		// position, scale
		infile.read(reinterpret_cast<char*>(&position), sizeof(Vec3));
		infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
		// worldMatrix
		worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));
		// mesh
		//readStringfromFile(infile, gemPath);
		//model.init(gemPath, core);
		// texture: diffus, normal
		for (auto& path : diffusePath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		for (auto& path : normalPath) {
			readStringfromFile(infile, path);
			textures.load(core, path);
		}
		// bool
		infile.read(reinterpret_cast<char*>(&insideAggroRange), sizeof(bool));
		infile.read(reinterpret_cast<char*>(&insideAttackRange), sizeof(bool));
	}

};

class Player : public NPC {
public:
	Player(DXcore* core, const Vec3 _position, const Vec3 _scale) {
		position = _position;
		scale = _scale;

		shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);
		model.init("Models/Soldier1.gem", core); 

		worldMatrix = Matrix::worldMatrix(position, scale ,Vec3(0,0,0));
		// initialize animation instance
		animationInstance.animation = &model.animation;
		animationInstance.currentAnimation = "idle";
		// load texture
		textures.load(core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png");
		textures.load(core, "Textures/MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");
		textures.load(core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Normal.png");
		textures.load(core, "Textures/MaleDuty_3_OBJ_Serious_Packed0_Normal.png");
	}

	void updatePos(Vec3 camPos) {
		position = camPos - Vec3(-0.8,9.5,-1.3);
		worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0,0,0));
	}

	void updateAnimationInstance(float dt) {
		// update current animation state 
		//animController.updateNPCState(insideAggroRange, insideAttackRange, health, dt);
		//animationInstance.update(animController.stateToString(), dt);
		animationInstance.update("rifle aiming idle", dt);
	}

};