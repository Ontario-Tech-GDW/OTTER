/*
Week 2 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
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

//TODO: Write templated LERP function.

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 800, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//TODO: Load in duck model and texture.

	//TODO: Set up camera entity and transform.

	//TODO: Create our duck entity.

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: LERP stuff!

		//TODO: Update camera, update duck transform, draw duck...

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
