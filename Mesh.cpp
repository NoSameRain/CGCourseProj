#include"Mesh.h"

void Mesh::init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indices;
	core->device->CreateBuffer(&bd, &data, &indexBuffer);
	bd.ByteWidth = vertexSizeInBytes * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertices;
	core->device->CreateBuffer(&bd, &data, &vertexBuffer);
	indicesSize = numIndices;
	strides = vertexSizeInBytes;
}

void Mesh::init_static(DXcore* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices)
{
	init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
}

void Mesh::init_animated(DXcore* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
{
	init(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
}

void Mesh::draw(DXcore* core) {
	UINT offsets = 0;
	core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	core->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	core->devicecontext->DrawIndexed(indicesSize, 0, 0);
	//OutputDebugString(L"Debug message: This is a test output\n");
}

void Model_static::init(std::string filename, DXcore* core) {
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	loader.load(filename, gemmeshes);

	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
			// AABB collision box
			colliBox.extend(v.pos);
		}
		textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
		textureFilenames.push_back(gemmeshes[i].material.find("normals").getValue());
		//debugLog("tex name: (" + gemmeshes[i].material.find("diffuse").getValue() + ")----------------------");
		// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
		mesh.init_static(core, vertices, gemmeshes[i].indices);

		meshes.push_back(mesh);
	}
}

void Model_static::bindTexture(DXcore* core, TextureManager* textures, Shader* shader, int index) {
	shader->updateTexturePS(core, 0, textures->find(textureFilenames[index * 2])); // albedo
	shader->updateTexturePS(core, 1, textures->find(textureFilenames[index * 2 + 1])); // normal
}

void Model_static::draw(DXcore* core) {
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(core);
	}
}

void Model_animated::init(std::string filename, DXcore* core) {
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	GEMLoader::GEMAnimation gemanimation;
	loader.load(filename, gemmeshes, gemanimation);

	//Vertices
	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<ANIMATED_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
			ANIMATED_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
			vertices.push_back(v);
			// AABB collision detection
			colliBox.extend(v.pos);
		}
		//path
		textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
		textureFilenames.push_back(gemmeshes[i].material.find("normals").getValue());
		// Load texture with filename: gemmeshes[i].material.find("diffuse").getValue()
		//debugLog("tex name: (" + gemmeshes[i].material.find("diffuse").getValue() + -------------------");

		mesh.init_animated(core, vertices, gemmeshes[i].indices);
		meshes.push_back(mesh);
	}
	//bones
	for (int i = 0; i < gemanimation.bones.size(); i++)
	{
		Bone bone;
		bone.name = gemanimation.bones[i].name;
		memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
		bone.parentIndex = gemanimation.bones[i].parentIndex;
		animation.skeleton.bones.push_back(bone);

	}
	// global inverse------------------------------------------------------------------------------------------
	animation.skeleton.globalInverse.identity();
	//animation.skeleton.globalInverse = gemanimation.globalInverse;

	//animations
	for (int i = 0; i < gemanimation.animations.size(); i++)
	{
		std::string name = gemanimation.animations[i].name;

		//debugLog("anim name: (" + name + ")");//---------------------------------debug------------------------------------

		AnimationSequence aseq;
		aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
		for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
		{
			AnimationFrame frame;
			for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
			{
				Vec3 p;
				Quaternion q;
				Vec3 s;
				memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
				frame.positions.push_back(p);
				memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
				frame.rotations.push_back(q);
				memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
				frame.scales.push_back(s);
			}
			aseq.frames.push_back(frame);
		}
		animation.animations.insert({ name, aseq });
	}
}

void Model_animated::bindTexture(DXcore* core, TextureManager* textures, Shader* shader, UINT startSlot) {//string texTypeName) {
	for (int i = 0; i < meshes.size(); i++)
	{
		//shader->updateTexturePS(core, texTypeName, textures->find(textureFilenames[i]));
		if (startSlot == 0) {
			shader->updateTexturePS(core, startSlot, textures->find(textureFilenames[i]));
			//debugLog("diffuse name: (" + textureFilenames[i] + ")----------------------");
		}
		else if (startSlot == 1) {
			shader->updateTexturePS(core, startSlot, textures->find(textureFilenames[i + 1]));
			//debugLog("normal name: (" + textureFilenames[i] + ")----------------------");
		}

	}
	//for (int i = 0; i < textureFilenames.size(); i++) {
	//	
	//	debugLog("tex name: (" + textureFilenames[i] + ")----------------------");
	//}
	//debugLog("---------------------------------");
}

void Model_animated::draw(DXcore* core) {
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(core);
	}
}

void Plane::init(DXcore* core) {
	std::vector<STATIC_VERTEX> vertices;
	vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
	vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
	vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
	vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
	std::vector<unsigned int> indices;
	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);

	mesh.init_static(core, vertices, indices);
}

void Plane::bindTexture(DXcore* core, TextureManager* textures, Shader* shader, std::string nameD, std::string nameN) {
	//shader->updateTexturePS(core, 0, textures->textures[nameD]->srv);
	//shader->updateTexturePS(core, 0, textures->textures[nameN]->srv);
	shader->updateTexturePS(core, 0, textures->find(nameD));
	shader->updateTexturePS(core, 1, textures->find(nameN));
	if (!textures->find(nameD)) {
		debugLog("Failed to find albedo texture");
	}
	if (!textures->find(nameN)) {
		debugLog("Failed to find normal texture");
	}

}

void Sphere::init(DXcore* core) {
	int rings = 90;
	int segments = 90;
	float radius = 180.f;
	std::vector<STATIC_VERTEX> vertices;
	for (int lat = 0; lat <= rings; lat++) {
		float theta = lat * 3.14159265358979323846f / rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		for (int lon = 0; lon <= segments; lon++) {
			float phi = lon * 2.0f * 3.14159265358979323846f / segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);
			Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
			Vec3 normal = position.normalize();
			float tu = 1.0f - (float)lon / segments;
			float tv = 1.0f - (float)lat / rings;
			vertices.push_back(addVertex(position, normal, tu, tv));
			// AABB collision detection
		}
	}
	std::vector<unsigned int> indices;
	for (int lat = 0; lat < rings; lat++)
	{
		for (int lon = 0; lon < segments; lon++)
		{
			int current = lat * (segments + 1) + lon;
			int next = current + segments + 1;
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}
	mesh.init_static(core, vertices, indices);
}
void Sphere::bindTexture(DXcore* core, TextureManager* textures, Shader* shader, std::string nameD) {
	//shader->updateTexturePS(core, 0, textures->find(nameD)); textures[name]->srv
	shader->updateTexturePS(core, 0, textures->textures[nameD]->srv);
	if (textures->textures[nameD]->srv == nullptr) {
		debugLog("Failed to find albedo texture: " + nameD);
	}

}