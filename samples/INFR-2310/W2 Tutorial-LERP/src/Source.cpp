/*
Tutorial 1 Sample - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//Templated LERP function.
//(This will work for any type that supports addition and scalar multiplication.)
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 800, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//Load in the ducky model.
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");

	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Endpoints for lerping our position.
	glm::vec3 origPos = glm::vec3(0.0f, -1.0f, 0.0f), newPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//Desired time for position lerp and timer.
	float posTimeLimit = 2.0f;
	float posTimer = 0.0f;
	//Whether we'll be going forward or backward.
	bool duckyGoPlaces = true;

	//Endpoints for lerping our colour.
	glm::vec3 origColor = glm::vec3(1.0f, 1.0f, 1.0f), newColor = glm::vec3(0.0f, 1.0f, 1.0f);
	//Desired time for colour lerp and timer.
	float colTimeLimit = 4.0f;
	float colTimer = 0.0f;
	//Whether we'll be going white or green.
	bool duckyGoGreen = true;

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//Increment timers.
	    posTimer += deltaTime;
		colTimer += deltaTime;

		//Resetting timers and adjusting back/forth booleans.
		if (posTimer >= posTimeLimit)
		{
			posTimer = 0.f;
			duckyGoPlaces = !duckyGoPlaces;
		}

		if (colTimer >= colTimeLimit)
		{
			colTimer = 0.f;
			duckyGoGreen = !duckyGoGreen;
		}
		
		//Calculate t.
		float tPos = posTimer / posTimeLimit;
		float tColor = colTimer / colTimeLimit;

		//If true the duck lerps to a new pos--otherwise it returns to the original pos.
		if (duckyGoPlaces)
		{
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else
		{
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}

		//If true the duck turns green, otherwise it lerps to its original colour (white).
		//(Material colour is multiplied with texture colour.)
		if (duckyGoGreen)
		{
			duckMat.m_color = LERP(origColor, newColor, tColor);
		}
		else
		{
			duckMat.m_color = LERP(newColor, origColor, tColor);
		}
		
		//Updates the camera.
		camEntity.Get<CCamera>().Update();

		//The duck's transformation matrix gets recomputed to make sure it's accurate.
		duckEntity.transform.RecomputeGlobal();

		//The duck is drawn.
		duckEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
