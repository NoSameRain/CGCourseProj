#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "levelManager.h"
#include "Collision.h"
#include "CollisionDetection.h"
#include "Utils.h"

extern "C" {
	__declspec(dllexport) extern DWORD NvOptimusEnablement;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{
	Window window;
	Camera camera;
	DXcore core;
	GamesEngineeringBase::Timer timer;
	Sampler sampler;
	LevelManager levelManager;

	bool ifCollided = false;

	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;
	Matrix view, projection, vp;

	window.create(1024, 1024, "My Window");
	core.init(1024, 1024, window.hwnd, false);

	float time = 0.f;
	float fov = 45.0f;

	// calculate projection matrix
	projection.perspectiveProjection(1024.f / 1024.f, tan(fov * 5 * M_PI / 180));
	// initialize sampler
	sampler.init(&core);
	// initaiize gameObect
	
	std::vector<std::string> treeTextures = {
	"Textures/bark07.png",
	"Textures/fir branch.png",
	"Textures/bark07_Normal.png",
	"Textures/fir branch_Normal.png"
	};
	Tree pine(&core, "Shaders/VertexShader_static.txt", "Models/pine1.gem", treeTextures,  Vec3(-80,0,-70), Vec3(0.05, 0.05, 0.05), 23);
	
	std::vector<std::string> flowerTextures = {
	"Textures/flower daisy.png",
	"Textures/plant05.png",
	"Textures/daisy leaf.png",
	"Textures/flower daisy_Normal.png",
	"Textures/plant05_Normal.png",
	"Textures/daisy leaf_Normal.png"
	};
	Foliage flower(&core, "Shaders/VertexShader_grass.txt", "Models/flower1.gem", flowerTextures, Vec3(-60, 0, -60), Vec3(0.08, 0.08, 0.08), 50);
	
	NPC npc(&core, Vec3(0, 0, -25), Vec3(2.2, 2.2, 2.2));
	//NPC npc1(&core, Vec3(-5, 0, -10), Vec3(1.9, 1.9, 1.9));
	Ground ground(&core, Vec3(0, 0, 0), Vec3(8.5, 1, 8.5));
	SkyBox skyBox(&core, Vec3(0, 0, 0), Vec3(1.2, 1, 1.2));
	Player player(&core, Vec3(0, 0, 0), Vec3(0.065, 0.065, 0.065));
	
	// bind sampler to s0 and s1: s0 for diffuse, s1 for normal map
	sampler.bind(&core, 0);
	sampler.bind(&core, 1);

	while (true) {
		window.processMessages();
		core.clear();

		if (window.keys[VK_ESCAPE]) {
			break;
		}

		float dt = timer.dt();

		//------------------------------------------CAMERA----------------------------------------------
		// collision detection: only detect tree and npc here
		ifCollided = collisionDetectionTree(player, pine) || collisionDetection(player, npc);// || collisionDetectionNPC(npc, npc1);
		if(ifCollided) debugLog("collided !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		// update camera -- first personal view
		camera.updateRotation(dt, window);
		camera.updateTranslation(dt, window,ifCollided);
		view = camera.getLookAtMat();
		vp =  view * projection;
		                                                                    
		// load and save data from custom file
		if (window.keys['O']) {
			//levelManager.saveLevelData(ground, pine, flower, npc, player);
		}
		if (window.keys['L']) {
			//levelManager.loadLevelData(&core, ground, pine, flower, npc, player);
		}

		//------------------------------------------DRAW------------------------------------------------
		
		// draw ground
		ground.draw(&core, vp);
		// draw sky box
		skyBox.draw(&core, vp);
		// draw foliage
		pine.draw(&core,vp,dt);
		flower.draw(&core, vp,dt);
		// draw npc
		npc.updateAnimationInstance(dt, camera.position);
		npc.draw(&core, vp);
		//npc1.updateAnimationInstance(dt, camera.position);
		//npc1.draw(&core, vp);
		// draw player
		player.updatePos(camera.position, pine);
		player.updateAnimationInstance(dt);
		player.draw(&core, vp);
		
		//pine.drawCollisionBox(&core, vp);
		//npc.drawCollisionBox(&core, vp);
		//npc1.drawCollisionBox(&core, vp);
		//player.drawCollisionBox(&core, vp);

		core.present();

		time += dt;
	}
}
