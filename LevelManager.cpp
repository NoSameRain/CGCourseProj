#include "levelManager.h"
void LevelManager::saveLevelData(Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        debugLog("can not open outfile");
        return;
    }
    // save date for all GameObjects in game level
    ground.saveDatatoFile(outfile);
    pine.saveDatatoFile(outfile);
    flower.saveDatatoFile(outfile);
    npc.saveDatatoFile(outfile);
    player.saveDatatoFile(outfile);

    outfile.close();
    debugLog("level saved succesfully!");
}
void LevelManager::loadLevelData(DXcore* core, Ground& ground, Foliage& pine, Foliage& flower, NPC& npc, Player& player) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        debugLog("can not open infile");
        return;
    }

    // load date for all GameObjects in game level
    ground.loadDataFromFile(core, infile);
    pine.loadDataFromFile(core, infile);
    flower.loadDataFromFile(core, infile);
    npc.loadDataFromFile(core, infile);
    player.loadDataFromFile(core, infile);

    infile.close();
    debugLog("level loaded succesfully!");
}