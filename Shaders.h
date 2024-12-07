#pragma once
#include"Device.h"
#include<fstream>
#include<string>
#include<sstream>
#include<D3D11.h>
#include<D3Dcompiler.h>
#include<d3d11shader.h>
#include"ShaderReflection.h"

#pragma comment(lib,"dxguid.lib")
using namespace std;


class Shader {
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* layout;

    //11.25 geometry
    std::vector<ConstantBuffer> psConstantBuffers;
    std::vector<ConstantBuffer> vsConstantBuffers;
    std::map<std::string, int> textureBindPointsVS;
    std::map<std::string, int> textureBindPointsPS;


    std::string getFileData(std::string filename);

    void loadVS(DXcore* core, std::string hlsl);

    void loadPS(DXcore* core, std::string hlsl);
   
    void init(std::string vs_filename, std::string ps_filename, DXcore* core);

    void updateConstantVS(std::string modelName, std::string bufferName, std::string variableName, void* data);

    //tex bind texture to t0 and t1 
    void updateTexturePS(DXcore* core, UINT startSlot, ID3D11ShaderResourceView* srv);

    void apply(DXcore* core);

};
