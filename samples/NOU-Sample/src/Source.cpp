/*
NOU Sample - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Source.cpp
Really simple demo showing off entity creation + model rendering.
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/Shader.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

int main()
{
	//Initialize our window.
	App::Init("NOU Sample", 600, 600);
	App::SetClearColor(glm::vec4(0.2f, 0.0f, 0.4f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//Manually specify data for a single triangle.
	//This lets us quickly test rendering without relying on a mesh loader.
	std::vector<glm::vec3> triangleVerts = 
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};

	std::vector<glm::vec2> triangleUVs = 
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	std::vector<glm::vec3> triangleNormals = 
	{
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	Mesh triangleMesh;
	triangleMesh.SetVerts(triangleVerts);
	triangleMesh.SetUVs(triangleUVs);
	triangleMesh.SetNormals(triangleNormals);

	//Load in a couple of GLTF models.
	Mesh boxMesh;
	GLTF::LoadMesh("models/boxtextured/BoxTextured.gltf", boxMesh);
	Mesh duckMesh;
	GLTF::LoadMesh("models/duck/Duck.gltf", duckMesh);

	//Load in our textures.
	Texture2D triangleTex = Texture2D("textures/color-grid.png");
	Texture2D duckTex = Texture2D("models/duck/DuckCM.png");
	Texture2D boxTex = Texture2D("models/boxtextured/CesiumLogoFlat.png");

	//Create materials for each of our models.
	Material triangleMat(prog_texLit);
	triangleMat.AddTexture("albedo", triangleTex);

	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	Material boxMat(prog_texLit);
	boxMat.AddTexture("albedo", boxTex);
	boxMat.m_color = glm::vec3(0.0f, 1.0f, 1.0f);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);
	
	//Set up an entity for each of our models.
	//Realistically, for a complex scene, you would specify transforms/models/etc. in a file
	//(e.g., XML) and write a loader to save/load your scenes.
	//This is hardcoded since this is just a simple demo where we want to manipulate
	//things easily and clearly for learning purposes.
	Entity triangleEntity = Entity::Create();
	triangleEntity.Add<CMeshRenderer>(triangleEntity, triangleMesh, triangleMat);
	triangleEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Entity boxEntity = Entity::Create();
	boxEntity.Add<CMeshRenderer>(boxEntity, boxMesh, boxMat);
	boxEntity.transform.m_pos = glm::vec3(-0.5f, 0.5f, 0.0f);
	boxEntity.transform.m_scale = glm::vec3(0.5f, 0.5f, 0.5f);
	boxEntity.transform.m_rotation = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * 
									 glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	float duckRotateSpeed = 45.0f;

	//Tick right before we enter our main loop (to make sure we don't have a huge
	//delta time jump during resource loading).
	App::Tick();

	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//Update camera to get the current VP matrix.
		camEntity.Get<CCamera>().Update();

		//Spin the duck.
		//Always spin the duck.
		float duckSpin = deltaTime * duckRotateSpeed;
		duckEntity.transform.m_rotation *= glm::angleAxis(glm::radians(duckSpin), glm::vec3(0.0f, 1.0f, 0.0f));

		//Update all our entity transforms (if we move things around,
		//we need to make sure that our transform matrices are up to date
		//so that objects will be drawn correctly!)
		triangleEntity.transform.RecomputeGlobal();
		duckEntity.transform.RecomputeGlobal();
		boxEntity.transform.RecomputeGlobal();

		//Draw all our entities.
		triangleEntity.Get<CMeshRenderer>().Draw();
		duckEntity.Get<CMeshRenderer>().Draw();
		boxEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	//Clean up our window, GLFW, etc.
	App::Cleanup();

	return 0;
}