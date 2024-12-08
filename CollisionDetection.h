#pragma once
#include "GameObject.h"

bool collisionDetection(Player& player, NPC& object);

bool collisionDetectionNPC(NPC& n1, NPC& n2);

bool collisionDetectionTree(Player& player, Tree& object);

bool collisionDetectionTreeNPC(NPC& n1, Tree& object);