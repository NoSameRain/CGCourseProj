#include "CollisionDetection.h"
#include "Utils.h"
bool collisionDetection(Player& player, NPC& object) {
	if (player.collisionBox.collisionCheck(object.collisionBox)) {
		debugLog("Collision detected between Player and NPC");
		return true;
	}
	//debugLog("No collision detected between Player and NPC");
	return false;
}
bool collisionDetectionTree(Player& player, Tree& object) {
	for (auto& box : object.collisionBoxes) {
		if (player.collisionBox.collisionCheck(box)) {
			debugLog("Collision detected between Player and Tree");
			return true;
		}
	}
	//debugLog("No collision detected between Player and Tree");
	return false;
}