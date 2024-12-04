#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "GameObject.h"

//std::wostringstream woss;
//void debugMessage(const std::wstring& message) {
//	OutputDebugString(message.c_str());
//}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{
	Window window;
	Camera camera;
	DXcore core;
	GamesEngineeringBase::Timer timer;
	Sampler sampler;

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
	Foliage pine(&core);
	NPC npc(&core);
	Ground ground(&core);
	
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
		//woss << L"mouse: (" << x << L", " << y << L")\n";
		//debugMessage(woss.str());

		//------------------------------------------DRAW------------------------------------------------
		sampler.bind(&core);

		// draw ground
		ground.draw(&core, vp);
		// draw tree
		pine.draw(&core,vp);
		// draw npc
		npc.updateAnimationInstance(dt, camera.position);
		npc.draw(&core, vp);

		core.present();

		time += dt;
	}
}
