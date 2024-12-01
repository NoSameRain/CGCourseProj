#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{
	Window win;
	DXcore core;
	Shader shader, shader_static;
	Model_static tree;
	Model_animated model;
	AnimationInstance instance;

	GamesEngineeringBase::Timer timer;
	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;
	Matrix worldMatrix1, worldMatrix2, worldMatrix_tree, worldMatrix_trex;
	Matrix view, projection, vp;
	Vec3 from;

	win.create(1024, 1024, "My Window");
	core.init(1024, 1024, win.hwnd, false);
	shader.init("VertexShader_anim.txt","PixelShader_anim.txt",&core);
	shader_static.init("VertexShader_1125.txt", "PixelShader_1125.txt", &core);
	tree.init("acacia_003.gem", &core);
	model.init("TRex.gem", &core);
	float time = 0.f;
	float fov = 45.0f;

	worldMatrix_tree = worldMatrix_tree.worldMatrix(Vec3(0, 0, 0), Vec3(0.01, 0.01, 0.01), 0, 0, 0);
	worldMatrix_trex = worldMatrix_tree.worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), 0, 0, 0);
	projection.perspectiveProjection(1024.f / 1024.f, tan(fov * 5 * M_PI / 180));

	//initialize animation instance
	instance.animation = &model.animation;  
	instance.currentAnimation = "Run";      

	while (true) {
		win.processMessages();
		core.clear();

		float dt = timer.dt();

		instance.update("Run", dt);


		from = Vec3(20 * cos(time), 10, 20 * sinf(time));
		//view = view.lookAtMat(Vec3(0,1,1), Vec3(0, 1, 0), Vec3(0, 1, 0));
		view = view.lookAtMat(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
		vp =  view * projection;

		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_tree);
		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		shader_static.apply(&core);
		tree.draw(&core);
		
		shader.updateConstantVS("Animated", "staticMeshBuffer", "bones", instance.matrices);
		shader.updateConstantVS("Animated", "staticMeshBuffer", "W", &worldMatrix_trex);
		shader.updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);

		shader.apply(&core);
		model.draw(&core);

		core.present();

		time += dt;
	}
}
