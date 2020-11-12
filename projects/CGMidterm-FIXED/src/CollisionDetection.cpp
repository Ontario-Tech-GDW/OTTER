#include "CollisionDetection.h"
#include "Gameplay/ScoreUI.h"

//Referenced from https://stackoverflow.com/questions/27342362/how-to-create-a-circle-collision-detection-c/27342421
//Referenced for creation of Game object class https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection


   bool CollisionDetection::CheckSphereCollision(GameObject& one, GameObject& two)
  {
     float distance = sqrt((two.position.x - one.position.x) * (two.position.x - one.position.x) + (two.position.y - one.position.y) * (two.position.y - one.position.y));

     if (distance <= one.radius + two.radius)
     {
         return true;
     }else {
        return false;
     }
  }

  objectTag CollisionDetection::CheckWallCollision(GameObject& puck)
  {

      glm::vec3 tempPosition = puck.GetPosition();

      if (tempPosition.y >= 4.442f){  
      return objectTag::BM_WALL;
      }
      if (tempPosition.y <= -4.751f){
      return objectTag::T_WALL;
      }
      if (tempPosition.x > 12.534f && !(tempPosition.y > -1.308 && tempPosition.y < 0.774)) {
      return objectTag::LS_WALL;
      }
      if (tempPosition.x < -12.63f && !(tempPosition.y > -1.308 && tempPosition.y < 0.774)) {
      return objectTag::RS_WALL;
      }
      if (tempPosition.y >= 4.442f) {
        //setTotalScore();
      }


      return objectTag::NONE;
  }

