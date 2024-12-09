#pragma once
#include <fstream>
#include "Camera.h"
#include "GameObject.h"
#include "Utils.h"

class LevelManager {
public:
    string filename = "savegame.dat";
    void saveLevelData(Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player);
    void loadLevelData(DXcore* core, Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player);

};


