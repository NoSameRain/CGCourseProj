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

        // ground
        ground.saveDatatoFile(outfile);

        // （2）
        //for (const auto& foliage : foliages) {
        //    Vec3 pos = foliage.position;
        //    Vec3 scale = foliage.scale;
        //    outfile.write(reinterpret_cast<const char*>(&pos), sizeof(Vec3));
        //    outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
        //    // 保存材质路径
        //    for (const std::string& texPath : foliage.textures) {
        //        size_t texPathLength = texPath.size();
        //        outfile.write(reinterpret_cast<const char*>(&texPathLength), sizeof(size_t));
        //        outfile.write(texPath.c_str(), texPathLength);
        //    }
        //}

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

        // 加载 Ground 对象信息
        //Vec3 position, scale;
        //infile.read(reinterpret_cast<char*>(&position), sizeof(Vec3));
        //infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
        //ground.worldMatrix = Matrix::worldMatrix(position, scale, 0, 0, 0);

        //size_t diffuseLength, normalLength;
        //infile.read(reinterpret_cast<char*>(&diffuseLength), sizeof(size_t));
        //char* diffusePath = new char[diffuseLength + 1];
        //infile.read(diffusePath, diffuseLength);
        //diffusePath[diffuseLength] = '\0';
        //ground.diffuseTex = std::string(diffusePath);
        //delete[] diffusePath;

        //infile.read(reinterpret_cast<char*>(&normalLength), sizeof(size_t));
        //char* normalPath = new char[normalLength + 1];
        //infile.read(normalPath, normalLength);
        //normalPath[normalLength] = '\0';
        //ground.normalTex = std::string(normalPath);
        //delete[] normalPath;

        //ground.textures.load(core, ground.diffuseTex);
        //ground.textures.load(core, ground.normalTex);

        //// 加载其他对象 (如 Foliage)
        //for (auto& foliage : foliages) {
        //    Vec3 pos, scale;
        //    infile.read(reinterpret_cast<char*>(&pos), sizeof(Vec3));
        //    infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
        //    foliage.position = pos;
        //    foliage.scale = scale;

        //    for (std::string& texPath : foliage.textures) {
        //        size_t texPathLength;
        //        infile.read(reinterpret_cast<char*>(&texPathLength), sizeof(size_t));
        //        char* texPathBuffer = new char[texPathLength + 1];
        //        infile.read(texPathBuffer, texPathLength);
        //        texPathBuffer[texPathLength] = '\0';
        //        texPath = std::string(texPathBuffer);
        //        delete[] texPathBuffer;

        //        foliage.textures.load(core, texPath);
        //    }
        //}

        infile.close();
        debugLog("level loaded succesfully!");
    }

};


