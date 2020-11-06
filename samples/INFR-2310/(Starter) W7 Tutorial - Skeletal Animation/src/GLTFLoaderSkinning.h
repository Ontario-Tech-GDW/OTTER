/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

GLTFLoaderSkinning.h
Utility functions for loading skinned models in the glTF format.
Helpful reference: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/

#pragma once

#include "SkinnedMesh.h"
#include "Animation.h"

#include <string>

//Forward declaration of TinyGLTF classes.
namespace tinygltf
{
	class Model;
	struct Primitive;
}

namespace nou::GLTF
{
	//Just some utility typedefs to make our loading functions a bit cleaner.
	typedef std::map<int, int> JointIndexLookup;
	typedef std::map<int, JointAnim*> JointAnimLookup;

	//Attributes that we support for joint animation.
	//(NONE is set as a flag for a type we don't support.)
	enum class ChannelType
	{
		NONE,
		POSITION,
		ROTATION
	};

	//Load a skinned mesh in base pose, including joint hierarchy and skin weights.
	void LoadSkinnedMesh(const std::string& filename, SkinnedMesh& mesh, 
					     bool flipUVY = true);
	
	//Grab a joint animation from a glTF file.
	void LoadAnimation(const std::string& filename, SkeletalAnim& anim);

	//Extract skeletal data (used by LoadSkinnedMesh).
	bool ExtractSkeleton(const tinygltf::Model& gltf, SkinnedMesh& mesh, 
						 JointIndexLookup& jointLookup,
						 std::string& err, std::string& warn);

	//Create a joint lookup used to "convert" glTF joint indices
	//to NOU joint indices.
	bool ExtractJointLookup(const tinygltf::Model& gltf, 
							JointIndexLookup& jointLookup,
							std::string& err, std::string& warn);

	//Extract animation data (used by LoadAnimation).
	bool ExtractSkeletalAnimation(const tinygltf::Model& gltf, SkeletalAnim& anim,
								  const JointIndexLookup& jointLookup,
								  std::string& err, std::string& warn);
}