#pragma once
#include<string>
class AnimationController {
public:
	float timer = 0.f;
	float duration = 1.f;
	enum state { Idle, roar, attack, death };
	state currentState, nextState;
	AnimationController() :currentState(Idle) {}

	void updateNPCState(const bool insideAggroRange, const bool insideAttackRange, const float health, float dt);

	std::string stateToString();

	void updatePlayerState(const bool insideAggroRange, const bool insideAttackRange, const float health, float dt);

};