#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "levelManager.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{
	Window window;
	Camera camera;
	DXcore core;
	GamesEngineeringBase::Timer timer;
	Sampler sampler;
	LevelManager levelManager;

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
	Foliage pine(&core, "Shaders/VertexShader_static.txt", "Models/pine.gem", "Textures/bark09.png", "Textures/stump01.png", "Textures/pine branch.png",
		"Textures/bark09_normal.png", "Textures/stump01_normal.png", "Textures/pine branch_Normal.png",  Vec3(-45,0,-40), Vec3(0.05, 0.05, 0.05), 30);
	Foliage flower(&core, "Shaders/VertexShader_grass.txt", "Models/flower1.gem", "Textures/flower daisy.png", "Textures/plant05.png", "Textures/daisy leaf.png",
		"Textures/flower daisy_Normal.png", "Textures/plant05_Normal.png", "Textures/daisy leaf_Normal.png", Vec3(-30, 0, -20), Vec3(0.08, 0.08, 0.08), 50);
	NPC npc(&core, Vec3(0, 0, 0), Vec3(2, 2, 2));
	Ground ground(&core, Vec3(0, 0, 0), Vec3(6, 1, 6));
	Player player(&core, Vec3(0, 0, 20), Vec3(0.06, 0.06, 0.06));
	
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
		// update camera -- first personal view
		camera.updateRotation(dt, window);
		camera.updateTranslation(dt, window);
		view = camera.getLookAtMat();
		vp =  view * projection;
		                                                                    
		// load and save data from custom file
		if (window.keys['O']) {
			levelManager.saveLevelData(ground, pine, flower, npc, player);
		}
		if (window.keys['L']) {
			levelManager.loadLevelData(&core, ground, pine, flower, npc, player);
		}

		//------------------------------------------DRAW------------------------------------------------
		
		// draw ground
		ground.draw(&core, vp);
		// draw foliage
		pine.draw(&core,vp,dt);
		flower.draw(&core, vp,dt);
		// draw npc
		npc.updateAnimationInstance(dt, camera.position);
		npc.draw(&core, vp);
		// draw player
		player.updatePos(camera.position);
		player.updateAnimationInstance(dt);
		player.draw(&core, vp);

		core.present();

		time += dt;
	}
}
