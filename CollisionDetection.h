#pragma once
#include "GameObject.h"

bool collisionDetection(Player& player, GameObject& object);

bool collisionDetectionNPC(NPC& n1, NPC& n2);

bool collisionDetectionTree(Player& player, Tree& object);