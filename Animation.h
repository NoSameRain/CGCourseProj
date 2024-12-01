#pragma once
#include"Device.h"
#include <map>

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
	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t));
	}
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::slerp(q1, q2, t);
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);
	}
	// Find frame given time
	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame;
		frame = min(frame, frames.size() - 1);
	}
	// Find next frame
	int nextFrame(int frame)
	{
		return min(frame + 1, frames.size() - 1);
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex)
	{
		Matrix scale = Matrix::scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
		// normalize?
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex].normalized(), frames[nextFrame(baseFrame)].rotations[boneIndex].normalized(), interpolationFact).toMatrix();
		Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));
		Matrix local = translation * rotation * scale;
		//Matrix local = scale * rotation * translation; //old
		if (skeleton->bones[boneIndex].parentIndex > -1)
		{
			Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;
			//Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex]; //old
			return global;
		}
		return local;
	}


};
//Stores skeleton and animations
class Animation {
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}
	// Calculate final transformation matrix
	void calcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < skeleton.bones.size(); i++)
		{
			matrices[i] = matrices[i] * skeleton.bones[i].offset  * skeleton.globalInverse;

		//matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse; //old

		}
	}


};

class AnimationInstance
{
public:
	Animation* animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[44];
	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation->animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}
	void update(std::string name, float dt) {

		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation->calcFrame(name, t, frame, interpolationFact);
		
		for (int i = 0; i < animation->skeleton.bones.size(); i++)
		{
			matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation->calcFinalTransforms(matrices);
	}

};