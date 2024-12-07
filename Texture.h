#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include"Device.h"
#include <string>
#include <map>

class Texture {
public:
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* srv;
    //DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    void init(DXcore* core, int width, int height, int channels, DXGI_FORMAT format, void* data);
    void load(DXcore* core, std::string filename);

    void free();
};

class Sampler {
public:
    ID3D11SamplerState* state;

    void init(DXcore* core);
    void bind(DXcore* core, UINT startSlot);
};

class TextureManager
{
public:
    std::map<std::string, Texture*> textures;
    void load(DXcore* core, std::string filename);
    ID3D11ShaderResourceView* find(std::string name);
    void unload(std::string name);
   /* ~TextureManager()
    {
        for (auto it = textures.cbegin(); it != textures.cend(); )
        {
            it->second->free();
            textures.erase(it++);
        }
    }*/
};
#endif