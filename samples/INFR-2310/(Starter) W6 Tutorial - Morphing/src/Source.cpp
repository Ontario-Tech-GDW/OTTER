/*
Week 6 Tutorial Sample - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/GLTFLoader.h"
#include "CMorphMeshRenderer.h"
#include "CMorphAnimator.h"

#include "imgui.h"

#include <memory>
#include <ctime>

using namespace nou;

//Forward declaring our global resources for this demo.
std::unique_ptr<ShaderProgram> prog_morph;
std::unique_ptr<Mesh> boiBase;
std::vector<std::unique_ptr<Mesh>> boiFrames;
std::unique_ptr<Material> boiMat;

//This function will load in our global resources.
//(It's only been separated to make main() a bit cleaner to look at.)
void LoadDefaultResources();

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	App::Init("Week 6 Tutorial - Morph Targets", 800, 800);
	App::SetClearColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	LoadDefaultResources();

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the Battery Boy entity.
	Entity boiEntity = Entity::Create();
	boiEntity.Add<CMorphMeshRenderer>(boiEntity, *boiBase, *boiMat);
	boiEntity.transform.m_scale = glm::vec3(0.01f, 0.01f, 0.01f);
	boiEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	boiEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//TODO: Add and set up our animator.

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		
		//Updates the camera.
		camEntity.Get<CCamera>().Update();

		boiEntity.transform.RecomputeGlobal();
		boiEntity.Get<CMorphMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
}

void LoadDefaultResources()
{
	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	//Lit and textured shader program.
	auto v_morph = std::make_unique<Shader>("shaders/morph.vert", GL_VERTEX_SHADER);
	auto f_lit   = std::make_unique<Shader>("shaders/lit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> morph = { v_morph.get(), f_lit.get() };
	prog_morph = std::make_unique<ShaderProgram>(morph);

	//Load in the base model.
	boiBase = std::make_unique<Mesh>();
	GLTF::LoadMesh("models/boi-t-pose.gltf", *boiBase);

	//Load in our other frames.
	std::string boiPrefix = "models/boi-";
	std::string filename;

	//Start by just loading in our two base frames.
	for (int i = 0; i <= 1; ++i)
	{
		filename = boiPrefix + std::to_string(i) + ".gltf";

		std::unique_ptr<Mesh> boiFrame = std::make_unique<Mesh>();
		GLTF::LoadMesh(filename, *boiFrame);

		boiFrames.push_back(std::move(boiFrame));
	}

	//Make material. 
	boiMat = std::make_unique<Material>(*prog_morph);
}