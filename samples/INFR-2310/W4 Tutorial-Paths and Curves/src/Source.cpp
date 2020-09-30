/*
Week 4 Tutorial Sample - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "CPathAnimator.h"
#include "Tools/PathUtility.h"
#include "NOU/GLTFLoader.h"

#include "imgui.h"

#include <memory>

using namespace nou;

//Forward declaring our global resources for this demo.
std::unique_ptr<ShaderProgram> prog_texLit, prog_lit, prog_unlit;
std::unique_ptr<Mesh> duckMesh, boxMesh;
std::unique_ptr<Texture2D> duckTex;
std::unique_ptr<Material> duckMat, unselectedMat, selectedMat, lineMat;

//This function will load in our global resources.
//(It's only been separated to make main() a bit cleaner to look at.)
void LoadDefaultResources();

//Templated LERP function.
//(This will work for any type that supports addition and scalar multiplication.)
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

//Templated Catmull-Rom function.
//(This will work for any type that supports addition and scalar multiplication.)
template<typename T>
T Catmull(const T& p0, const T& p1, const T& p2, const T& p3, float t)
{
	return 0.5f * (2.f * p1 + t * (-p0 + p2)
		+ t * t * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3)
		+ t * t * t * (-p0 + 3.f * p1 - 3.f * p2 + p3));
}

//TODO (For part of this week's task): Implement your Bezier function.
//Remember, p1 and p2 are your control handles, and the segment
//generated will fall between p0 and p3.
//It is important you follow this logic for treating your points
//for the utility drawing function to work for you.
//(Unless you'd like to modify the utility function yourself, that is :P).
template<typename T>
T Bezier(const T& p0, const T& p1, const T& p2, const T& p3, float t)
{
	return p0;
}

int main()
{
	App::Init("Week 4 Tutorial - Paths and Curves", 800, 800);
	App::SetClearColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	App::InitImgui();

	LoadDefaultResources();

	PathSampler::Bezier = Bezier<glm::vec3>;
	PathSampler::Catmull = Catmull<glm::vec3>;

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, *duckMesh, *duckMat);
	duckEntity.Add<CPathAnimator>(duckEntity);
	duckEntity.transform.m_scale = glm::vec3(0.003f, 0.003f, 0.003f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Setting up our container of waypoints.
	std::vector<std::unique_ptr<Entity>> points;

	//Setting up our utility to draw the given path.
	PathSampler sampler = PathSampler();
	
	Entity pathDrawUtility = Entity::Create();
	pathDrawUtility.Add<CLineRenderer>(pathDrawUtility, sampler, *lineMat);

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//Updates the camera.
		camEntity.Get<CCamera>().Update();

		//Update our path animator.
		duckEntity.Get<CPathAnimator>().Update(points, deltaTime);

		//Update transformation matrices.
		for (size_t i = 0; i < points.size(); ++i)
		{
			points[i]->transform.RecomputeGlobal();
		}

		duckEntity.transform.RecomputeGlobal();

		//Draw everything.
		for (size_t i = 0; i < points.size(); ++i)
		{
			points[i]->Get<CMeshRenderer>().Draw();
		}

		duckEntity.Get<CMeshRenderer>().Draw();

		//Draw our path (for debugging/demo purposes).
		pathDrawUtility.Get<CLineRenderer>().Draw(points);

		//For Imgui...
		App::StartImgui();

		static bool listPanel = true;
		ImGui::Begin("Waypoints", &listPanel, ImVec2(300, 200));

		//Add a new waypoint!
		if (ImGui::Button("Add"))
		{
			points.push_back(Entity::Allocate());
			auto& p = points.back();
			p->Add<CMeshRenderer>(*p, *boxMesh, *unselectedMat);
			p->transform.m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
			
			//Initialize our position somewhere near the last waypoint.
			if (points.size() > 1)
			{
				auto& lastP = points[points.size() - 2];
				p->transform.m_pos = lastP->transform.m_pos + glm::vec3(0.2f, 0.0f, 0.0f);
			}
		}
		
		//Remove the last waypoint.
		if (ImGui::Button("Remove") && points.size() > 0)
		{
			points.pop_back();
		}

		//Interface for selecting a waypoint.
		static size_t pointSelected = 0;
		static std::string pointLabel = "";

		if(pointSelected >= points.size())
			pointSelected = points.size() - 1;

		for (size_t i = 0; i < points.size(); ++i)
		{
			pointLabel = "Point " + std::to_string(i);

			if (ImGui::Selectable(pointLabel.c_str(), i == pointSelected))
			{
				if(pointSelected < points.size())
					points[pointSelected]->Get<CMeshRenderer>().SetMaterial(*unselectedMat);
			
				pointSelected = i;
			}	
		}

		ImGui::End();

		//Interface for moving a selected component.
		if (pointSelected < points.size())
		{
			auto& transform = points[pointSelected]->transform;

			points[pointSelected]->Get<CMeshRenderer>().SetMaterial(*selectedMat);
			static bool transformPanel = true;

			ImGui::Begin("Point Coordinates", &transformPanel, ImVec2(300, 100));

			//This will tie the position of the selected
			//waypoint to input fields rendered with Imgui.
			ImGui::InputFloat("X", &(transform.m_pos.x));
			ImGui::InputFloat("Y", &(transform.m_pos.y));
			ImGui::InputFloat("Z", &(transform.m_pos.z));

			ImGui::End();
		}

		App::EndImgui();

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
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> texLit = { v_texLit.get(), f_texLit.get() };
	prog_texLit = std::make_unique<ShaderProgram>(texLit);

	//Lit but not textured shader program.
	auto v_lit = std::make_unique<Shader>("shaders/lit.vert", GL_VERTEX_SHADER);
	auto f_lit = std::make_unique<Shader>("shaders/lit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> lit = { v_lit.get(), f_lit.get() };
	prog_lit = std::make_unique<ShaderProgram>(lit);

	//Unlit and not textured shader program.
	auto v_unlit = std::make_unique<Shader>("shaders/unlit.vert", GL_VERTEX_SHADER);
	auto f_unlit = std::make_unique<Shader>("shaders/unlit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> unlit = { v_unlit.get(), f_unlit.get() };
	prog_unlit = std::make_unique<ShaderProgram>(unlit);

	//Load in the ducky model.
	duckMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("duck/Duck.gltf", *duckMesh);
	duckTex = std::make_unique<Texture2D>("duck/DuckCM.png");

	boxMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("box/Box.gltf", *boxMesh);

	duckMat = std::make_unique<Material>(*prog_texLit);
	duckMat->AddTexture("albedo", *duckTex);

	unselectedMat = std::make_unique<Material>(*prog_lit);
	unselectedMat->m_color = glm::vec3(0.8f, 0.8f, 0.8f);

	selectedMat = std::make_unique<Material>(*prog_lit);
	selectedMat->m_color = glm::vec3(0.0f, 1.0f, 0.0f);

	lineMat = std::make_unique<Material>(*prog_unlit);
	lineMat->m_color = glm::vec3(1.0f, 1.0f, 1.0f);
}