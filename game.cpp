#include "Window.h"
#include "Device.h"
#include "Adapter.h"
#include "Shaders.h"
#include "Mesh.h"
#include "mathLibrary.h"
#include "GamesEngineeringBase.h"



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXcore core;
	Shader shader;
	Sphere sphere;
	Cube cube;
	Model model;
	GamesEngineeringBase::Timer timer;
	float WIDTH = 1024.f;
	float HEIGHT = 1024.f;
	Matrix worldMatrix1, worldMatrix2, worldMatrix_meshes;
	Matrix view, projection, vp;
	Vec3 from;

	win.create(1024, 1024, "My Window");
	core.init(1024, 1024, win.hwnd, false);
	shader.init("VertexShader_1125.txt","PixelShader_1125.txt",&core);
	sphere.init(&core);
	cube.init(&core);
	model.init("acacia_003.gem", &core);
	float time = 0.f;
	float fov = 45.0f;
	//worldMatrix1 = worldMatrix1.worldMatrix(Vec3(10, 0, 0), Vec3(0.5, 0.5, 0.5), 0, 0, 0);
	//worldMatrix2 = worldMatrix1.worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), 0, 0, 0);
	worldMatrix_meshes = worldMatrix1.worldMatrix(Vec3(0, 0, 0), Vec3(0.01, 0.01, 0.01), 0, 0, 0);
	projection.perspectiveProjection(1024.f / 1024.f, tan(fov * 0.5 * M_PI / 180));

	while (true) {
		win.processMessages();
		core.clear();

		float dt = timer.dt();
		from = Vec3(20 * cos(time), 10, 20 * sinf(time));
		view = view.lookAtMat(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
		vp = view * projection;

		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix1);
		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		//shader.apply(&core);
		//sphere.mesh.draw(&core);

		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix2);
		//shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);

		//shader.apply(&core);
		//cube.mesh.draw(&core);

		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_meshes);
		shader.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
		shader.apply(&core);
		model.draw(&core);

		core.present();

		time += dt;
	}
}
