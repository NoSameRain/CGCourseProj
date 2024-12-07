#pragma once
#include"Device.h"
#include <map>
#include<string>

struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};

struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};

//Frame holds values for each bone
struct AnimationFrame
{
	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};

//Stores animation data for one animation
class AnimationSequence {
public:
	std::vector<AnimationFrame> frames;
	float ticksPerSecond;
	Vec3 interpolate(Vec3 p1, Vec3 p2, float t);
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t);
	float duration();
	// Find frame given time
	void calcFrame(float t, int& frame, float& interpolationFact);
	// Find next frame
	int nextFrame(int frame);

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex);
};
//Stores skeleton and animations
class Animation {
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact);
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex);
	// Calculate final transformation matrix
	void calcFinalTransforms(Matrix* matrices);
};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[256];
	void resetAnimationTime();
	bool animationFinished();
	void update(std::string name, float dt);
};



