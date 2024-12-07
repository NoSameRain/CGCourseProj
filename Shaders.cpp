#include"Shaders.h"

std::string Shader::getFileData(std::string filename) {
    ifstream infile(filename);
    std::stringstream data;
    data << infile.rdbuf();
    return data.str();
}

void Shader::loadVS(DXcore* core, std::string hlsl)
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

    //layout description
   //D3D11_INPUT_ELEMENT_DESC layoutDesc_static[] =
   //{
   //    { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
   //    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
   //    { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
   //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
   //};

    D3D11_INPUT_ELEMENT_DESC layoutDesc_animated[] = {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 								D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 							D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    //core->device->CreateInputLayout(layoutDesc_static, 4, shader->GetBufferPointer(), shader->GetBufferSize(), &layout);
    core->device->CreateInputLayout(layoutDesc_animated, 6, shader->GetBufferPointer(), shader->GetBufferSize(), &layout);

    // extract constant buffer information
    ConstantBufferReflection reflection;
    reflection.build(core, shader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);

}

void Shader::loadPS(DXcore* core, std::string hlsl)
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

void Shader::init(std::string vs_filename, std::string ps_filename, DXcore* core) {
    string vsdata = getFileData(vs_filename);
    string psdata = getFileData(ps_filename);
    loadVS(core, vsdata);
    loadPS(core, psdata);
    //message box

}

void Shader::updateConstantVS(std::string modelName, std::string bufferName, std::string variableName, void* data) {
    // Find the constant buffer associated with the vertex shader
    for (int i = 0; i < vsConstantBuffers.size(); i++) {
        vsConstantBuffers[i].update(variableName, data);
    }
}

//tex bind texture to t0 and t1 
void Shader::updateTexturePS(DXcore* core, UINT startSlot, ID3D11ShaderResourceView* srv) {
    core->devicecontext->PSSetShaderResources(startSlot, 1, &srv);
}

void Shader::apply(DXcore* core) {
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