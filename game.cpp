#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{
	Window window;
	Camera camera;
	DXcore core;
	GamesEngineeringBase::Timer timer;

	Sampler sampler;
	Shader shader_animated, shader_static, shader_ground;
	TextureManager textures_pine, textures_trex;
	AnimationInstance instance;

	// Models
	Model_static tree;
	Plane plane;
	Model_animated trex;

	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;
	Matrix worldMatrix_plane, worldMatrix_tree, worldMatrix_trex;
	Matrix view, projection, vp;

	window.create(1024, 1024, "My Window");
	core.init(1024, 1024, window.hwnd, false);

	// init shaders
	shader_animated.init("Shaders/VertexShader_anim.txt","Shaders/PixelShader.txt",&core);
	shader_static.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader.txt", &core);
	shader_ground.init("Shaders/VertexShader_static.txt", "Shaders/PixelShader_ground.txt", &core);

	// init models
	tree.init("Models/pine.gem", &core);
	plane.init(&core);
	trex.init("Models/TRex.gem", &core);

	float time = 0.f;
	float fov = 45.0f;

	// calculate world matrix and projection matrix
	worldMatrix_tree = Matrix::worldMatrix(Vec3(-4, 0, -4), Vec3(0.03, 0.03, 0.03), 0, 0, 0);
	worldMatrix_trex = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), 0, 0, 0);
	worldMatrix_plane = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(10, 10, 10), 0, 0, 0);

	projection.perspectiveProjection(1024.f / 1024.f, tan(fov * 5 * M_PI / 180));

	// initialize animation instance
	instance.animation = &trex.animation;  
	instance.currentAnimation = "Run";   

	// initialize sampler
	sampler.init(&core);
	
	// load textures
	textures_pine.load(&core, "Textures/bark09.png");
	textures_pine.load(&core, "Textures/pine branch.png");
	textures_pine.load(&core, "Textures/stump01.png");

	textures_trex.load(&core, "Textures/T-rex_Base_Color.png");

	while (true) {
		window.processMessages();
		core.clear();

		float dt = timer.dt();

		instance.update("Run", dt);

		//------------------------------------------CAMERA----------------------------------------------
		// update camera -- first personal view
		camera.updateMovement(dt, window);
		// update LookAtMatrix using camera instance to calculate vp matrix
		view = view.lookAtMat(camera.position, camera.position + camera.forward, camera.up);
		vp =  view * projection;

		//------------------------------------------DRAW------------------------------------------------
		sampler.bind(&core);
		
		
		// draw ground
		shader_ground.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_plane);
		shader_ground.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader_ground.apply(&core);
		plane.mesh.draw(&core);
		
		// draw tree

		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_tree);
		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader_static.apply(&core);
		tree.draw(&core, &textures_pine, &shader_static);
		
		// draw trex
		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "bones", instance.matrices);
		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "W", &worldMatrix_trex);
		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);

		shader_animated.apply(&core);
		trex.draw(&core, &textures_trex, &shader_animated);

		


		core.present();

		time += dt;
	}
}
