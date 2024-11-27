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


    std::string getFileData(std::string filename) {
        ifstream infile(filename);
        std::stringstream data;
        data << infile.rdbuf();
        return data.str();
    }

    //11.25
    void loadVS(DXcore* core, std::string hlsl)
    {
        ID3DBlob* shader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &shader, &status);
        if (FAILED(hr))
        {
            printf("%s\n", (char*)status->GetBufferPointer());
            exit(0);
        }
        core->device->CreateVertexShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &vertexShader);

        // layout description
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
        {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        core->device->CreateInputLayout(layoutDesc, 4, shader->GetBufferPointer(), shader->GetBufferSize(), &layout);

        // extract constant buffer information
        ConstantBufferReflection reflection;
        reflection.build(core, shader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
        
    }

    void loadPS(DXcore* core, std::string hlsl)
    {
        ID3DBlob* shader;
        ID3DBlob* status;
        HRESULT hr = D3DCompile(hlsl.c_str(), strlen(hlsl.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
        if (FAILED(hr))
        {
            printf("%s\n", (char*)status->GetBufferPointer());
            exit(0);
        }
        core->device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, &pixelShader);
        ConstantBufferReflection reflection;
        reflection.build(core, shader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
    }
   
	void init(std::string vs_filename,std::string ps_filename, DXcore* core ) {
        string vsdata = getFileData(vs_filename);
        string psdata = getFileData(ps_filename);
        loadVS(core,vsdata);
        loadPS(core,psdata);

        //message box

	}

    void updateConstantVS(std::string modelName, std::string bufferName, std::string variableName, void* data) {
        // Find the constant buffer associated with the vertex shader
        for (int i = 0; i < vsConstantBuffers.size(); i++) {
            //if (vsConstantBuffers[i].name == bufferName) {
                vsConstantBuffers[i].update(variableName, data);
            //}
        }
    }

    void apply(DXcore* core) {
        core->devicecontext->IASetInputLayout(layout);
        core->devicecontext->VSSetShader(vertexShader, NULL, 0);
        core->devicecontext->PSSetShader(pixelShader, NULL, 0);

        for (int i = 0; i < vsConstantBuffers.size(); i++) {
            vsConstantBuffers[i].upload(core);
        }
        for (int i = 0; i < psConstantBuffers.size(); i++) {
            psConstantBuffers[i].upload(core);
        }

    }

};
