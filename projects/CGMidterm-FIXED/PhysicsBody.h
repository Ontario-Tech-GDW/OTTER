//
//#pragma once
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <GLM/glm.hpp>
//#include <GLM/gtc/matrix_transform.hpp>
//#include <GLM/gtc/type_ptr.hpp>
//
//#include "GameObject.h"
//
////#include "JSON.h"
////#include "Vector.h"
////#include "VertexManager.h"
////#include "EntityIdentifier.h"
////#include "Transform.h"
//
//	enum class BodyType
//	{
//		BOX,
//		CIRCLE,
//
//		NUM_TYPES
//	};
//
//	class PhysicsBody
//	{
//	public:
//		PhysicsBody() { };
//
//
//		//Consturcts a circle collider
//		PhysicsBody(GameObject* body, float radius, glm::vec2 centeroffSet, bool  isDynamic);
//		//Constructs a box collider
//		PhysicsBody(GameObject* body, float width, float height, glm::vec2 centerOffSet, bool isDynamic);
//
//		//Update
//		void Update(Transform* trans);
//
//		//Apply a force to the physics body
//		void ApplyForce(glm::vec3 force);
//
//		//Initializes body for drawing
//	/*	void InitBody();
//
//		void DrawBody();*/
//
//		//Getters
//		//Get the Box2D physics body
//		GameObject* GetBody() const;
//		//Get position of body
//		GameObject* GetPosition() const;
//		//Gets current force
//		glm::vec3 GetForce() const;
//		//Gets the current acceleration
//		glm::vec3 GetAcceleration() const;
//		//Gets the max velocity for each individual axis
//		float GetMaxVelo() const;
//		//Gets the current velocity
//		glm::vec3 GetVelocity() const;
//		//Gets the coefficient of friction
//		float GetFriction() const;
//
//		//what is the current gravitational acceleration?
//		static glm::vec3 GetGravityAcceleration();
//
//		bool GetGravity() const;
//
//		//Set the mass of the physics body
//		float GetMass() const;
//
//		//Get the body type enum
//		BodyType GetBodyType() const;
//		//Gets the center offset for the body
//		//*if the offset is 0,0, then all corners will be relative to the center of the actual sprite
//
//		glm::vec2 GetCenterOffset() const;
//		//Gets the corners of the body
//		glm::vec2 GetBottomLeft() const;
//		glm::vec2 GetBottomRight() const;
//		glm::vec2 GetTopLeft() const;
//		glm::vec2 GetTopRight()const;
//		//Gets the width of the physics body
//		float GetWidth() const;
//		//Gets the height of the physics body
//		float GetHeight() const;
//		//Gets the radius of the physics body
//		float GetRadius() const;
//		//Get whether or not we are currently drawing our physics bodies
//		static bool GetDraw();
//
//		//Gets this body's type
//		//*This can be the environment, enemy, player, friendly
//		unsigned int GetBodyID() const;
//
//		//What types does this body collide with?
//		//*Can be environment, enemy, player and friendly
//		unsigned int GetCollideID() const;
//
//		//Get whether or not this object is dynamic (aka it moves or not)
//		bool GetDynamic() const;
//
//
//
//
//
//
//		//Setters
//		//Sets the pointer to the box2D body
//		void SetBody(GameObject* body);
//		//Set position (just sets the variable, dosen't actually ste the position)
//		void SetPosition(GameObject bodyPos);
//		//Sets force to be applied to this body
//		void SetForce(glm::vec3 force);
//		//Sets the acceleration to be applied to this body
//		void SetAcceleration(glm::vec3 accel);
//		//Sets Max velocity for the individual axes
//		void SetMaxVelo(float velo);
//		//Sets the velocity of the physics body
//		void SetVelocity(glm::vec3 velo);
//
//		//Sets the coefficient of friction
//		void SetFriction(float fric);
//
//		//what is the current gravitational acceleration?
//		static void SetGravityAcceleration(glm::vec3 grav);
//
//		void SetGravity(bool grav);
//
//		//Set the mass of the physics body
//		void SetMass(float mass);
//
//		//Get the body type enum
//		void SetBodyType(BodyType type);
//		//Sets the center offset for the body
//		//*if the offset is 0,0, then all corners will be relative to the center of the actual sprite
//
//		void SetCenterOffset(glm::vec2 cent);
//		//Gets the corners of the body
//		void SetBottomLeft(glm::vec2 BL);
//		void SetBottomRight(glm::vec2 BR);
//		void SetTopLeft(glm::vec2 TL);
//		void SetTopRight(glm::vec2 TR);
//		//Gets the width of the physics body
//		void SetWidth(float width);
//		//Gets the height of the physics body
//		void  SetHeight(float height);
//		//Gets the radius of the physics body
//		void SetRadius(float radius);
//		//Set whether the bodies are being drawn or not
//		static void SetDraw(bool drawBodies);
//
//		//Gets this body's type
//		//*This can be the environment, enemy, player, friendly
//		void SetBodyID(unsigned int bodyID);
//
//
//		//What types does this body collide with?
//		//*Can be environment, enemy, player and friendly
//		void SetCollideID(unsigned int collideID);
//
//		//Get whether or not this object is dynamic (aka it moves or not)
//		void SetDynamic(bool isDynamic);
//
//		//
//
//	private:
//
//
//
//		//The actual box2D body
//		GameObject* m_body = nullptr;
//		GameObject m_position = GameObject(0.f, 0.f);
//
//		//The applied force
//		glm::vec3 m_appliedForce = glm::vec3(0.f, 0.f, 0.f);
//		//The frictional force
//		glm::vec3 m_frictionForce = glm::vec3(0.f, 0.f, 0.f);
//		//Stores in the net force
//		glm::vec3 m_netForce = glm::vec3(0.f, 0.f, 0.f);
//		//Stores the acceleration
//		glm::vec3 m_acceleration = glm::vec3(0.f, 0.f, 0.f);
//		//Max velocity in each axes
//		float m_maxVelo = 20.f;
//		//Stores the velocity
//		glm::vec3 m_velocity = glm::vec3(0.f, 0.f, 0.f);
//
//		//Coefficient of friction
//		float m_friction = 0.35f;
//
//		//The acceleration due to gravity * mass
//		static glm::vec3 m_gravityAcceleration;
//		//Is gravity being applied to this body?
//		bool m_applyGravity = false;
//
//		//Mass of the body
//		float m_mass = 1.f;
//
//		//Body type
//		BodyType m_bodyType = BodyType::CIRCLE;
//
//		//How far from the center of the sprite is it
//		glm::vec2 m_centerOffSet = glm::vec2();
//		//Each corner
//		glm::vec2 m_bottomLeft = glm::vec2();
//		glm::vec2 m_bottomRight = glm::vec2();
//		glm::vec2 m_topleft = glm::vec2();
//		glm::vec2 m_topRight = glm::vec2();
//		//Width and hegith of the body
//		float m_width = 0.f;
//		float m_height = 0.f;
//
//		float m_radius = 0.f;
//
//		//the type of this physics body
//		unsigned int m_bodyID;
//		//The types of bodies this body collides with
//		unsigned int m_collideID;
//
//		//Is the object non-moving
//		bool m_dynamic = false;
//
//		//Do you draw bodies?
//		static bool m_drawBodies;
//
//		//Physics body drawing stuff
//		GLuint m_vao = GL_NONE;
//		GLuint m_vboPose = GL_NONE;
//
//
//
//
//
//
//	};
//
//
//	struct CollisionIDs
//	{
//	public:
//		//Gets the different IDs
//		static unsigned int Player();
//		static unsigned int Enemy();
//
//	private:
//		//Holds the different IDs
//		static unsigned int m_playerID;
//		static unsigned int m_environmentID;
//		static unsigned int m_enemyID;
//	};
//
//	//Send body TO json file
//	inline void to_json(nlohmann::json& j, const PhysicsBody& phys)
//	{
//		//Postion
//		j["BodyPosition"] = { phys.GetPosition().x, phys.GetPosition().y };
//
//		//Stores body type
//		j["BodyType"] = phys.GetBodyType();
//		//Center offset
//		j["CenterOffset"] = { phys.GetCenterOffset().x, phys.GetCenterOffset().y };
//		//Stores corners
//		j["BottomLeft"] = { phys.GetBottomLeft().x, phys.GetBottomLeft().y };
//		j["BottomRight"] = { phys.GetBottomRight().x, phys.GetBottomRight().y };
//		j["TopLeft"] = { phys.GetTopLeft().x, phys.GetTopLeft().y };
//		j["TopRight"] = { phys.GetTopRight().x, phys.GetTopRight().y };
//
//		//height and width
//		j["Width"] = phys.GetWidth();
//		j["Height"] = phys.GetHeight();
//		//Radius
//		j["Radius"] = phys.GetRadius();
//
//		//Max velocity
//		j["MaxVelo"] = phys.GetMaxVelo();
//
//		//Friction strength
//		j["Friction"] = phys.GetFriction();
//
//		//Gravity enabled
//		j["Gravity"] = phys.GetGravity();
//
//		//Collision type (body id) and what this body collides *with*
//		j["BodyID"] = phys.GetBodyID();
//		j["CollideID"] = phys.GetCollideID();
//
//		//Sets whether the object is moving
//		j["IsDynamic"] = phys.GetDynamic();
//
//
//	}
//
//	//Reads bodi in FROM json file
//	inline void from_json(const nlohmann::json& j, PhysicsBody& phys)
//	{
//
//		//Set postion
//		phys.SetPosition(b2Vec2(j["BodyPosition"][0], j["bodyPosition"][1]));
//
//		//Stes body type
//		phys.SetBodyType(j["BodyType"]);
//		//sets the center offset
//		phys.SetCenterOffset(vec2(j["CenterOffset"][0], j["CenterOffset"][1]));
//
//		//Sets corners
//		phys.SetBottomLeft(vec2(j["BottomLeft"][0], j["BottomLeft"][1]));
//		phys.SetBottomRight(vec2(j["BottomRight"][0], j["BottomRight"][1]));
//		phys.SetTopLeft(vec2(j["TopLeft"][0], j["TopLeft"][1]));
//		phys.SetTopRight(vec2(j["TopRight"][0], j["TopRight"][1]));
//
//		//Set width and height
//		phys.SetWidth(j["Width"]);
//		phys.SetHeight(j["Height"]);
//		//Set Radius
//		phys.SetRadius(j["Radius"]);
//
//		//Sets Max Velocity
//		phys.SetMaxVelo(j["MaxVelo"]);
//
//		//Set does gravity affect this body?
//		phys.SetGravity(j["Gravity"]);
//
//		//Set collision type (body id) and what this body collides *with*
//		phys.SetBodyID(j["BodyID"]);
//		phys.SetCollideID(j["CollideID"]);
//
//		//Set whther or not the object is moving
//		phys.SetDynamic(j["IsDynamic"]);
//
//	}
//
//};
//
