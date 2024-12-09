#pragma once
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "AnimationController.h"
#include "Collision.h"
#include "Utils.h"

class GameObject {
public:
	Vec3 position;
	Vec3 scale;
	Matrix worldMatrix;
	Shader shader;
	TextureManager textures;
	// store file path of diffuse,normal textures
	std::vector<std::string> diffusePath, normalPath;  
	// store file path of .GEM file
	std::string gemPath;
	// store min and max value of AABB
	AABB collisionBox;
	
	GameObject(){}
	virtual ~GameObject() {} 
	virtual void draw(DXcore* core, Matrix& vp){}
	virtual void saveDatatoFile(ofstream& outfile) = 0;
	virtual void loadDataFromFile(DXcore* core, ifstream& infile) = 0;
	void drawCollisionBox(DXcore* core, Matrix& vp);
};

class Ground :public GameObject {
public:
	Plane plane;
	Ground(DXcore* core, const Vec3 _position, const Vec3 _scale);

	void draw(DXcore* core, Matrix& vp)override;
	void saveDatatoFile(ofstream& outfile)override;
	void loadDataFromFile(DXcore* core, ifstream& infile)override;
};

class SkyBox :public GameObject {
public:	
	Sphere sphere;
	float timer = 0.f;
	SkyBox(DXcore* core, const Vec3 _position, const Vec3 _scale);
	void draw(DXcore* core, Matrix& vp)override;
	void saveDatatoFile(ofstream& outfile) override {}
	void loadDataFromFile(DXcore* core, ifstream& infile) override {}
};

class Foliage : public GameObject {
public:	
	Model_static model;
	std::vector<Vec3> positions;
	std::vector<Matrix> worldMatrices;
	int num;
	float timer = 0.f;
	Foliage(){}
	Foliage(DXcore* core, std::string VertexSHaderName, std::string gemName, const std::vector<std::string>& texturePaths, Vec3 _position, Vec3 _scale, int _num);

	void draw(DXcore* core, Matrix& vp, float dt);

	void saveDatatoFile(ofstream& outfile)override;

	void loadDataFromFile(DXcore* core, ifstream& infile)override;
};

class Tree : public Foliage {
public:
	std::vector<AABB> collisionBoxes;
	Tree(DXcore* core, std::string VertexSHaderName, std::string gemName, const std::vector<std::string>& texturePaths, Vec3 _position, Vec3 _scale, int _num);
	void drawCollisionBox(DXcore* core, Matrix& vp);
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
	NPC(DXcore* core, const Vec3 _position, const Vec3 _scale);

	void checkInsideAggroRange(Vec3 playerPos);

	void checkInsideAttackRange(Vec3 playerPos);

	void updateAnimationInstance(float dt, Vec3 playerPos);

	void draw(DXcore* core, Matrix& vp)override;

	void saveDatatoFile(ofstream& outfile)override;

	void loadDataFromFile(DXcore* core, ifstream& infile)override;
	
};

class Player : public NPC {
public:
	Player(DXcore* core, const Vec3 _position, const Vec3 _scale);

	void updatePos(Vec3 camPos, GameObject& object);

	void updateAnimationInstance(float dt);
	
};