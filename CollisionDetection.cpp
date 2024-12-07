#include "CollisionDetection.h"
bool collisionDetection(Player& player, GameObject& object) {
	if (player.model.colliBox.collisionCheck(object.collisionBox)) {
		return true;
	}
	return false;
}
bool collisionDetectionTree(Player& player, Tree& object) {
	for (auto& box : object.collisionBoxes) {
		if (player.model.colliBox.collisionCheck(box)) {
			return true;
		}
	}
	return false;
}