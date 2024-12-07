#pragma once
#include <fstream>
#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "GameObject.h"
#include"Utils.h"

class LevelManager {
public:
    string filename = "savegame.dat";
    void saveLevelData(Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player) {
        std::ofstream outfile(filename, std::ios::binary);
        if (!outfile.is_open()) {
            debugLog("can not open outfile");
            return;
        }
        ground.saveDatatoFile(outfile);
        //pine.saveDatatoFile(outfile);
        flower.saveDatatoFile(outfile);
        npc.saveDatatoFile(outfile);
        player.saveDatatoFile(outfile);

        outfile.close();
        debugLog("level saved succesfully!");
    }
    void loadLevelData(DXcore* core, Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player) {
        std::ifstream infile(filename, std::ios::binary);
        if (!infile.is_open()) {
            debugLog("can not open infile");
            return;
        }

        // ground
        ground.loadDataFromFile(core, infile);
        //pine.loadDataFromFile(core, infile);
        flower.loadDataFromFile(core, infile);
        npc.loadDataFromFile(core, infile);
        player.loadDataFromFile(core, infile);
        
        infile.close();
        debugLog("level loaded succesfully!");
    }

};


