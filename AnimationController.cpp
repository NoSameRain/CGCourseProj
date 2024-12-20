#include"AnimationController.h"
#include<string>
void AnimationController::updateNPCState(const bool insideAggroRange, const bool insideAttackRange, const float health, float dt) {
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
	// make the animation transitions smoother
	if (currentState != nextState) {
		timer += dt;
		if (timer >= duration) {
			currentState = nextState;
			timer = 0.f;
		}
	}

}

std::string AnimationController::stateToString() {
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

void AnimationController::updatePlayerState(const bool insideAggroRange, const bool insideAttackRange, const float health, float dt) {

}