#include "Animation.h"
#include <string>

Vec3 AnimationSequence::interpolate(Vec3 p1, Vec3 p2, float t) {
	return ((p1 * (1.0f - t)) + (p2 * t));
}
Quaternion AnimationSequence::interpolate(Quaternion q1, Quaternion q2, float t) {
	return Quaternion::slerp(q1, q2, t);
}
float AnimationSequence::duration() {
	return ((float)frames.size() / ticksPerSecond);
}
// Find frame given time
void AnimationSequence::calcFrame(float t, int& frame, float& interpolationFact)
{
	interpolationFact = t * ticksPerSecond;
	frame = (int)floorf(interpolationFact);
	interpolationFact = interpolationFact - (float)frame;
	frame = min(frame, frames.size() - 1);
}
// Find next frame
int AnimationSequence::nextFrame(int frame)
{
	return min(frame + 1, frames.size() - 1);
}

Matrix AnimationSequence::interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex)
{
	Matrix scale = Matrix::scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
	// normalize?
	Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrame(baseFrame)].rotations[boneIndex], interpolationFact).toMatrix();
	Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));
	Matrix local = scale * rotation * translation;
	if (skeleton->bones[boneIndex].parentIndex > -1)
	{
		// Global Matrix=Parent Bone Matrix×Local Matrix
		Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
		return global;
	}
	return local;
}

void Animation::calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
	animations[name].calcFrame(t, frame, interpolationFact);
}
Matrix Animation::interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
	return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
}
// Calculate final transformation matrix
void Animation::calcFinalTransforms(Matrix* matrices)
{
	for (int i = 0; i < skeleton.bones.size(); i++)
	{
		// Final Matrix=Global Matrix×Inverse Bind Pose×Global Inverse
		matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;

	}
}

void AnimationInstance::resetAnimationTime()
{
	t = 0;
}

bool AnimationInstance::animationFinished()
{
	if (t > animation->animations[currentAnimation].duration())
	{
		return true;
	}
	return false;
}

void AnimationInstance::update(std::string name, float dt) {

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