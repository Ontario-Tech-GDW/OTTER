#include "CollisionDetection.h"

    /*
    
    https://gamedevelopment.tutsplus.com/tutorials/when-worlds-collide-simulating-circle-circle-collisions--gamedev-769
     https://stackoverflow.com/questions/27342362/how-to-create-a-circle-collision-detection-c/27342421
    https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
    Used these references for the circle circle collision
    
    */

//Sphere-Sphere collision
   bool CollisionDetection::CheckSphereCollision(GameObject& one, GameObject& two)
  {
     float distance = sqrt((two.position.x - one.position.x) * (two.position.x - one.position.x) + (two.position.y - one.position.y) * (two.position.y - one.position.y));

     if (distance < one.radius + two.radius){
        float overlap = (one.radius + two.radius) - distance;
        one.UpdatePosition(glm::vec3(one.GetPosition().x + overlap, one.GetPosition().y + overlap, one.GetPosition().z));
     }

     if (distance <= one.radius + two.radius)
     {
         return true;
     }else {
        return false;
     }
  }

   //THIS LOGIC WAS REFERENCED FROM OUR UNITY TUTORIALS IN INTERMEDIATE GAME DESIGN

   //Sphere-Wall collision using tags
  objectTag CollisionDetection::CheckWallCollision(GameObject& puck, Puck p)
  {

      glm::vec3 tempPosition = puck.GetPosition();

      
      if (tempPosition.y >= 4.505f)
      { 
          p.setPosition(glm::vec3(tempPosition.x, std::clamp(tempPosition.y, -4.710f, 4.710f), tempPosition.z));
          return objectTag::BM_WALL;
      }
      if (tempPosition.y <= -4.751f)
      {
          p.setPosition(glm::vec3(tempPosition.x, std::clamp(tempPosition.y, -4.710f, 4.710f), tempPosition.z));
          return objectTag::T_WALL;
      }
      if (tempPosition.x > 12.534f && !(tempPosition.y > -1.308 && tempPosition.y < 0.774)) 
      {
          p.setPosition(glm::vec3(std::clamp(tempPosition.x, -12.534f, 12.534f), tempPosition.y, tempPosition.z));
          return objectTag::LS_WALL;
      }
      if (tempPosition.x < -12.63f && !(tempPosition.y > -1.308 && tempPosition.y < 0.774))
      {
          p.setPosition(glm::vec3(std::clamp(tempPosition.x, -12.534f, 12.534f), tempPosition.y, tempPosition.z));
          return objectTag::RS_WALL;
      }

      return objectTag::NONE;
  }

