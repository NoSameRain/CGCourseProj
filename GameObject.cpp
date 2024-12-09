#include "GameObject.h"

void GameObject::drawCollisionBox(DXcore* core, Matrix& vp) {
	Cube box;
	box.init(core, collisionBox);
	shader.init("Shaders/VertexShader_box.txt", "Shaders/PixelShader_box.txt", core);
	worldMatrix = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0)); //?
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
	shader.apply(core);
	box.mesh.draw(core);

}

Ground::Ground(DXcore* core, const Vec3 _position, const Vec3 _scale) {
	position = _position;
	scale = _scale;
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));

	// init shader
	shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader.txt", core);

	// init mesh (plane)
	plane.init(core);

	// load texture
	diffusePath.push_back("Textures/Grass001_1K-PNG_Color.png");
	normalPath.push_back("Textures/Grass001_1K-PNG_NormalDX.png");
	textures.load(core, diffusePath[0]);
	textures.load(core, normalPath[0]);
}

void Ground::draw(DXcore* core, Matrix& vp) {
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

	// bind texture to t0 and t1
	plane.bindTexture(core, &textures, &shader, diffusePath[0], normalPath[0]);
	shader.apply(core);
	plane.mesh.draw(core);
}

void Ground::saveDatatoFile(ofstream& outfile) {
	// position, scale
	outfile.write(reinterpret_cast<const char*>(&position), sizeof(Vec3));
	outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
	// texture: diffus, normal
	for (auto& path : diffusePath) {
		writeStringToFile(outfile, path);
	}
	for (auto& path : normalPath) {
		writeStringToFile(outfile, path);
	}
	// shader? plane?
}

void Ground::loadDataFromFile(DXcore* core, ifstream& infile) {
	// position, scale
	infile.read(reinterpret_cast<char*>(&position), sizeof(Vec3));
	infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
	// worldMatrix
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));
	// texture: diffus, normal
	for (auto& path : diffusePath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
	for (auto& path : normalPath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
	// shader? plane?
}

SkyBox::SkyBox(DXcore* core, const Vec3 _position, const Vec3 _scale) {
	position = _position;
	scale = _scale;
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, M_PI));

	// init shader
	shader.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader_albedo.txt", core);

	// init mesh (sphere)
	sphere.init(core);

	// load texture
	textures.load(core, "Textures/kloppenheim_06_puresky.png");
}

void SkyBox::draw(DXcore* core, Matrix& vp) {
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
	shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

	// bind texture to t0 and t1
	sphere.bindTexture(core, &textures, &shader, "Textures/kloppenheim_06_puresky.png");
	shader.apply(core);
	sphere.mesh.draw(core);
}

Foliage::Foliage(DXcore* core, std::string VertexSHaderName, std::string gemName, const std::vector<std::string>& texturePaths, Vec3 _position, Vec3 _scale, int _num) {
	position = _position;
	scale = _scale;
	num = _num;

	// generate worldMatrices for random foliages
	for (int i = 0; i < num; i++) {
		int seedX = rand() % 140;
		int seedZ = rand() % 120;
		Vec3 pos = position + Vec3(seedX, 0, seedZ);
		positions.push_back(pos);
		worldMatrices.push_back(Matrix::worldMatrix(pos, scale, Vec3(0, 0, 0)));
	}

	// init mesh (.gem file)
	gemPath = gemName;
	model.init(gemPath, core);

	// init shader
	shader.init(VertexSHaderName, "Shaders/PixelShader.txt", core);

	// load texture
	for (const auto& texPath : texturePaths) {
		textures.load(core, texPath);
	}
}

void Foliage::draw(DXcore* core, Matrix& vp, float dt) {
	timer += dt;
	for (int i = 0; i < num; i++) {
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrices[i]);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "time", &timer);

		for (int meshIndex = 0; meshIndex < model.meshes.size(); meshIndex++) {
			// bind texture to t0 and t1
			model.bindTexture(core, &textures, &shader, meshIndex);
			shader.apply(core);
			model.meshes[meshIndex].draw(core);
		}
	}
}

void Foliage::saveDatatoFile(ofstream& outfile) {
	// write number of foliages in the scene to file
	outfile.write(reinterpret_cast<const char*>(&num), sizeof(int));
	// write position, scale value for every foliage (with same scale value)
	for (auto& pos : positions) {
		outfile.write(reinterpret_cast<const char*>(&pos), sizeof(Vec3));
	}
	outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
	// write .GEM mesh file path
	writeStringToFile(outfile, gemPath);
	// write texture file path: diffuse, normal 
	for (auto& path : diffusePath) {
		writeStringToFile(outfile, path);
	}
	for (auto& path : normalPath) {
		writeStringToFile(outfile, path);
	}

}

void Foliage::loadDataFromFile(DXcore* core, ifstream& infile) {
	// read number of foliages in the scene from file
	infile.read(reinterpret_cast<char*>(&num), sizeof(int));

	// read position, scale value for every foliage (with same scale value)
	positions.clear();
	Vec3 pos;
	while (infile.read(reinterpret_cast<char*>(&pos), sizeof(Vec3))) {
		positions.push_back(pos);
	}
	infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));

	// recalculate worldMatrix by position and scale loaded
	worldMatrices.clear();
	for (int i = 0; i < num; i++) {
		worldMatrices.push_back(Matrix::worldMatrix(positions[i], scale, Vec3(0, 0, 0)));
	}

	// read .GEM mesh file path and re-init model
	readStringfromFile(infile, gemPath);
	model.init(gemPath, core);

	// read texture file path: diffuse, normal and reload texture
	for (auto& path : diffusePath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
	for (auto& path : normalPath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
}

Tree::Tree(DXcore* core, std::string VertexSHaderName, std::string gemName, const std::vector<std::string>& texturePaths, Vec3 _position, Vec3 _scale, int _num) {
	position = _position;
	scale = _scale;
	num = _num;

	// generate worldMatrices for random foliages
	for (int i = 0; i < num; i++) {
		int seedX = rand() % 140;
		int seedZ = rand() % 120;
		Vec3 pos = position + Vec3(seedX, 0, seedZ);
		positions.push_back(pos);
		worldMatrices.push_back(Matrix::worldMatrix(pos, scale, Vec3(0, 0, 0)));
		// collision box scale
		model.colliBox.transformAABB(Vec3(scale.x, scale.y, scale.z * 0.3), positions[i]);
		collisionBoxes.push_back(model.colliBox);
	}

	// init mesh (.gem file)
	gemPath = gemName;
	model.init(gemPath, core);

	// init shader
	shader.init(VertexSHaderName, "Shaders/PixelShader.txt", core);

	// load texture
	for (const auto& texPath : texturePaths) {
		textures.load(core, texPath);
	}
}

void Tree::drawCollisionBox(DXcore* core, Matrix& vp) {
	for (const auto& cbox : collisionBoxes) {
		Cube box;
		box.init(core, cbox);
		shader.init("Shaders/VertexShader_box.txt", "Shaders/PixelShader_box.txt", core);
		worldMatrix = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0)); //?
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shader.apply(core);
		box.mesh.draw(core);
	}

}

NPC::NPC(DXcore* core, const Vec3 _position, const Vec3 _scale) {
	position = _position;
	scale = _scale;
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));

	health = 100.f;
	aggroRange = 50.f;
	attackRange = 35.f;
	insideAggroRange = false;
	insideAttackRange = false;

	// init mesh (.gem file)
	gemPath = "Models/TRex.gem";
	model.init(gemPath, core);

	// collision box transform
	model.colliBox.transformAABB(Vec3(scale.x * 0.7, scale.y, scale.z * 0.8), position);
	collisionBox = model.colliBox;

	// init shader
	shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);

	// initialize animation instance
	animationInstance.animation = &model.animation;
	animationInstance.currentAnimation = "Idle";
	// load texture
	textures.load(core, "Textures/T-rex_Base_Color.png");
	textures.load(core, "Textures/T-rex_Normal_OpenGL.png");
}

void NPC::checkInsideAggroRange(Vec3 playerPos) {
	if (position.distance(playerPos) <= aggroRange) insideAggroRange = true;
	else insideAggroRange = false;
}

void NPC::checkInsideAttackRange(Vec3 playerPos) {
	if (position.distance(playerPos) <= attackRange) insideAttackRange = true;
	else insideAttackRange = false;
}

void NPC::updateAnimationInstance(float dt, Vec3 playerPos) {
	checkInsideAggroRange(playerPos);
	checkInsideAttackRange(playerPos);
	// update current animation state 
	animController.updateNPCState(insideAggroRange, insideAttackRange, health, dt);
	animationInstance.update(animController.stateToString(), dt);
}

void NPC::draw(DXcore* core, Matrix& vp) {
	shader.updateConstantVS("Animated", "staticMeshBuffer", "bones", animationInstance.matrices);
	shader.updateConstantVS("Animated", "staticMeshBuffer", "W", &worldMatrix);
	shader.updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);

	// bind texture to t0 and t1: t0 for diffuse, t1 for normal map
	model.bindTexture(core, &textures, &shader, 0);
	model.bindTexture(core, &textures, &shader, 1);

	shader.apply(core);
	model.draw(core);
}

void NPC::saveDatatoFile(ofstream& outfile) {
	// position, scale
	outfile.write(reinterpret_cast<const char*>(&position), sizeof(Vec3));
	outfile.write(reinterpret_cast<const char*>(&scale), sizeof(Vec3));
	// mesh
	//writeStringToFile(outfile, gemPath);

	// texture: diffus, normal
	for (auto& path : diffusePath) {
		writeStringToFile(outfile, path);
	}
	for (auto& path : normalPath) {
		writeStringToFile(outfile, path);
	}
	// bool
	outfile.write(reinterpret_cast<const char*>(&insideAggroRange), sizeof(bool));
	outfile.write(reinterpret_cast<const char*>(&insideAttackRange), sizeof(bool));
	// mesh
	writeStringToFile(outfile, gemPath);
}

void NPC::loadDataFromFile(DXcore* core, ifstream& infile) {
	// position, scale
	infile.read(reinterpret_cast<char*>(&position), sizeof(Vec3));
	infile.read(reinterpret_cast<char*>(&scale), sizeof(Vec3));
	// worldMatrix
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));
	// mesh
	//readStringfromFile(infile, gemPath);
	//model.init(gemPath, core);
	// texture: diffus, normal
	for (auto& path : diffusePath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
	for (auto& path : normalPath) {
		readStringfromFile(infile, path);
		textures.load(core, path);
	}
	// bool
	infile.read(reinterpret_cast<char*>(&insideAggroRange), sizeof(bool));
	infile.read(reinterpret_cast<char*>(&insideAttackRange), sizeof(bool));
}

Player::Player(DXcore* core, const Vec3 _position, const Vec3 _scale) {
	position = Vec3(0, 12, 25) - Vec3(-0.8, 12, 12);// _position;
	scale = _scale;
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));

	// init shader
	shader.init("Shaders/VertexShader_anim.txt", "Shaders/PixelShader.txt", core);

	// init model 
	gemPath = "Models/Soldier1.gem";
	model.init(gemPath, core);

	// collision box transform
	model.colliBox.transformAABB(Vec3(scale.x * 0.5, scale.y, scale.z * 0.8), position);
	collisionBox = model.colliBox;

	// initialize animation instance
	animationInstance.animation = &model.animation;
	animationInstance.currentAnimation = "idle";

	// load texture
	textures.load(core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png");
	textures.load(core, "Textures/MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");
	textures.load(core, "Textures/MaleDuty_3_OBJ_Happy_Packed0_Normal.png");
	textures.load(core, "Textures/MaleDuty_3_OBJ_Serious_Packed0_Normal.png");
}

void Player::updatePos(Vec3 camPos, GameObject& object) {
	Vec3 position_previous = position;
	position = camPos - Vec3(-0.8, 14, 12); // z -1.3 15
	worldMatrix = Matrix::worldMatrix(position, scale, Vec3(0, 0, 0));

	Vec3 translation = position - position_previous;
	collisionBox.updateAABB(translation);
}

void Player::updateAnimationInstance(float dt) {
	// update current animation state 
	//animController.updateNPCState(insideAggroRange, insideAttackRange, health, dt);
	//animationInstance.update(animController.stateToString(), dt);
	animationInstance.update("jump loop", dt); //jump loop rifle aiming idle

}