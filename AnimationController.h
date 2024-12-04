#pragma once
class AnimationController {
public:

	enum state { Idle, roar, attack, death };
	state currentState, nextState;
	AnimationController() :currentState(Idle) {}

	void updateNPCState(const bool insideAggroRange, const bool insideAttackRange, const float health) {
		switch (currentState) {
		case Idle:
			if (insideAggroRange) {
				nextState = roar;
			}
			if (insideAttackRange) {
				nextState = attack;
			}
			break;
		case roar:
			if (insideAttackRange) {
				nextState = attack;
			}
			if (!insideAggroRange) {
				nextState = Idle;
			}
			break;
		case attack:
			if (health <= 0) {
				nextState = death;
			}
			else if (!insideAttackRange) {
				nextState = roar;
			}
			break;
		case death:
			break;
		}
		if (currentState != nextState) currentState = nextState;
	}

	std::string stateToString() {
		switch (currentState) {
		case Idle:
			return "Idle";
		case roar:
			return "roar";
		case attack:
			return "attack";
		case death:
			return "death";
		}
	}

};