/*
Week 3 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "CKnightFSM.h"

#include "imgui.h"

#include <memory>

using namespace nou;

int main() 
{
	App::Init("Week 3 Tutorial - Sprites", 800, 800);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load in some shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);

	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });

	//Load in sprites.
	Texture2D boomTex = Texture2D("explosion.png", true);
	Texture2D knightTex = Texture2D("knight.png", true);

	Material boomMat(prog_sprite);
	boomMat.AddTexture("albedo", boomTex);

	Material knightMat(prog_sprite);
	knightMat.AddTexture("albedo", knightTex);
	
	//Load in explosion spritesheet, add animation.
	auto boomSheet = std::make_unique<Spritesheet>(boomTex, glm::vec2(222.0f, 222.0f));
	boomSheet->AddAnimation("boom", 0, 27, 30.0f);
	boomSheet->SetDefaultFrame(27);

	//Load in knight spritesheet, add animations.
	auto knightSheet = std::make_unique<Spritesheet>(knightTex, glm::vec2(64.0f, 64.0f));
	knightSheet->AddAnimation(CKnightFSM::idleClip, 0, 4, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::runClip, 5, 12, 12.0f);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//Create the explosion entity.
	Entity okBoomer = Entity::Create();
	okBoomer.Add<CSpriteRenderer>(okBoomer, *boomSheet, boomMat);
	auto& boomAnim = okBoomer.Add<CSpriteAnimator>(okBoomer, *boomSheet);

	//Create the knight entity.
	Entity knightEntity = Entity::Create();
	knightEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	knightEntity.Add<CSpriteRenderer>(knightEntity, *knightSheet, knightMat);
	knightEntity.Add<CSpriteAnimator>(knightEntity, *knightSheet);
	knightEntity.Add<CKnightFSM>(knightEntity);

	App::Tick();

	//Disabling the depth buffer.
	//(The reason we do this is so we can naively draw sprites on top
	//of each other in the order we call Draw().)
	glDisable(GL_DEPTH_TEST);

	//Disable backface culling.
	//(The reason we disable culling is to make it simpler for us to 
	//flip sprites without making OpenGL think we're drawing polygons
	//that shouldn't be visible.)
	glDisable(GL_CULL_FACE);
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//If we're providing input from the arrow keys, the knight is moving.
		bool moving = Input::GetKey(GLFW_KEY_RIGHT) || Input::GetKey(GLFW_KEY_LEFT);
		knightEntity.Get<CKnightFSM>().SetVariable("moving", moving);

		if (moving)
		{
			//Whether or not we're moving left (should flip the sprite's orientation).
			bool flip = Input::GetKey(GLFW_KEY_LEFT);

			//Sets direction knight is facing.
			knightEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			//Move the knight.
			knightEntity.transform.m_pos.x += (flip) ? -100.0f * deltaTime : 100.0f * deltaTime;
		}

		//Updates all the entities.
		camEntity.Get<CCamera>().Update();
		okBoomer.Get<CSpriteAnimator>().Update(deltaTime);
		knightEntity.Get<CKnightFSM>().Update();
		knightEntity.Get<CSpriteAnimator>().Update(deltaTime);
		
		//Recomputes global matrices.
		okBoomer.transform.RecomputeGlobal();
		knightEntity.transform.RecomputeGlobal();

		//Draws the sprites.
		okBoomer.Get<CSpriteRenderer>().Draw();
		knightEntity.Get<CSpriteRenderer>().Draw();	

		//For Imgui stuff...
		App::StartImgui();

		static bool panelOpen = true;
		ImGui::Begin("The Lil' ImGUI(TM) Panel That Could", &panelOpen, ImVec2(300, 100));
		
		if (ImGui::Button("Boom!"))
			okBoomer.Get<CSpriteAnimator>().PlayOnce("boom");

		ImGui::End();
		App::EndImgui();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
