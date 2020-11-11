/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

GLTFLoaderSkinning.cpp
Utility functions for loading skinned models in the glTF format.
Helpful reference: https://www.khronos.org/files/gltf20-reference-guide.pdf
*/

#include "GLTFLoaderSkinning.h"
#include "NOU/GLTFLoader.h"

#include "tiny_gltf.h"
#include "GLM/gtx/matrix_decompose.hpp"

namespace nou::GLTF
{
	void LoadSkinnedMesh(const std::string& filename, SkinnedMesh& mesh, 
						 bool flipUVY)
	{
		auto gltf = std::make_unique<tinygltf::Model>();

		std::string err, warn;

		//Step 1: Load our glTF file via TinyGLTF.
		bool result = ParseGLTF(filename, *gltf, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		//Step 2: Extract mesh geometry (verts, normals, UVs).
		result = ExtractGeometry(*gltf, mesh, flipUVY, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		//Step 3: Extract our skeletal data.
		JointIndexLookup jointLookup;
			
		result = ExtractSkeleton(*gltf, mesh, jointLookup, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		//Step 4: Extract skin weights.
		result = ExtractSkinWeights(*gltf, mesh, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		DumpErrorsAndWarnings(filename, err, warn);

		printf("Loaded skinned mesh from %s.\n", filename.c_str());
	}

	void LoadAnimation(const std::string& filename, SkeletalAnim& anim)
	{
		auto gltf = std::make_unique<tinygltf::Model>();

		std::string err, warn;

		//Step 1: Load our glTF file via TinyGLTF.
		bool result = ParseGLTF(filename, *gltf, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		//Step 2: Build a joint lookup that allows us to 
		//align joint indices in the file with joint indices
		//as specified in a NOU skeleton.
		JointIndexLookup jointLookup;

		result = ExtractJointLookup(*gltf, jointLookup, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		//Step 3: Grab our animation.
		result = ExtractSkeletalAnimation(*gltf, anim, jointLookup, err, warn);

		if (!result)
		{
			DumpErrorsAndWarnings(filename, err, warn);
			return;
		}

		printf("Loaded animation clip from %s.\n", filename.c_str());
	}

	bool ExtractSkeleton(const tinygltf::Model& gltf, SkinnedMesh& mesh,
					     JointIndexLookup& jointLookup,
					     std::string& err, std::string& warn)
	{
		if (gltf.skins.size() == 0)
		{
			err = "No skeleton data in file.";
			return false;
		}

		const tinygltf::Skin& skin = gltf.skins[0];

		if (skin.joints.size() == 0)
		{
			err = "No joints specified in skeleton.";
			return false;
		}

		Skeleton& skeleton = mesh.m_skeleton;

		//Resize our collection of joints to match the skeleton in the file.
		skeleton.m_joints.resize(skin.joints.size());

		DataGetter invBindGetter = BuildGetter(gltf, skin.inverseBindMatrices);

		if (invBindGetter.elementSize != 16 * sizeof(float))
		{
			err = "Inverse bind matrices are in a currently unsupported format. " \
				  "Consider changing your GLTF export settings, or else check for " \
				  "and support this format in your GLTF loader implementation.";

			return false;
		}

		//Find our root node.
		int root_id = skin.skeleton;

		if (root_id == -1)
		{
			//Some converters might not explicitly specify a root.
			//To make FK work, we need to make an educated guess.
			warn += "\nFile does not specify a root skeleton node. " \
				   "Assuming minimum joint index as root.";

			root_id = INT_MAX;

			//We already checked to make sure this container has
			//at least one element.
			for (size_t i = 0; i < skin.joints.size(); ++i)
			{
				if(skin.joints[i] < root_id)
					root_id = skin.joints[i];
			}
		}

		glm::mat4 jointMat = glm::mat4();
		glm::vec3 jointScale;
		glm::vec3 jointSkew;
		glm::vec4 jointPersp;
		
		//Grab all the joint transform data for our skeleton.
		for (size_t i = 0; i < skin.joints.size(); ++i)
		{
			//The current joint...
			Joint& joint = skeleton.m_joints[i];
			joint.m_owner = &skeleton;

			int j_id = skin.joints[i];

			//If we're the root, set ourselves up as the root.
			if (j_id == root_id)
				skeleton.m_rootInd = static_cast<int>(i);

			//Register our NOU index (i) as corresponding with
			//the glTF node index (j_id).
			jointLookup[j_id] = static_cast<int>(i);

			//The node representing our joint in the glTF hierarchy...
			const tinygltf::Node& node = gltf.nodes[skin.joints[i]];
			joint.m_name = node.name;

			//If the glTF file specifies position/rotation separately...
			if (node.translation.size() >= 3 && node.rotation.size() >= 4)
			{
				joint.m_basePos = glm::vec3(static_cast<float>(node.translation[0]),
					static_cast<float>(node.translation[1]),
					static_cast<float>(node.translation[2]));

				joint.m_pos = joint.m_basePos;

				//VERY IMPORTANT: glTF will specify quaternions in XYZW order.
				//GLM specifies quaternions in WXYZ order.
				//Any time a quaternion is "translated" into GLM, make sure to 
				//switch the order of components accordingly.
				joint.m_baseRotation = glm::quat(static_cast<float>(node.rotation[3]),
					static_cast<float>(node.rotation[0]),
					static_cast<float>(node.rotation[1]),
					static_cast<float>(node.rotation[2]));

				joint.m_rotation = joint.m_baseRotation;
			}
			else if (node.matrix.size() == 16)
			{
				for (int i = 0; i < 4; ++i)
				{
					jointMat[i] = glm::vec4(static_cast<float>(node.matrix[i*4]),
											static_cast<float>(node.matrix[i*4+1]),
											static_cast<float>(node.matrix[i*4+2]),
											static_cast<float>(node.matrix[i*4+3]));
				}

				//We're just using glm::decompose here since it's built-in with GLM
				//and straightforward (we could write our own function
				//to grab rotation and position alone, since that's all
				//we need, if we make some assumptions).
				glm::decompose(jointMat, jointScale,
							   joint.m_baseRotation, joint.m_basePos,
							   jointSkew, jointPersp);

				joint.m_rotation = joint.m_baseRotation;
				joint.m_pos = joint.m_basePos;
			}
			else
			{
				err = "Joint transforms are in a currently unsupported format." \
					  "Consider changing your GLTF export settings, or else check for " \
					  "and support this format in your GLTF loader implementation.";
				
				return false;
			}

			memcpy(&joint.m_invBind,
				   &invBindGetter.data[i * invBindGetter.stride],
				   16 * sizeof(GLfloat));
		}

		//Set up the parent-child relationship between all our joints.
		for (size_t i = 0; i < skin.joints.size(); ++i)
		{
			int j_id = skin.joints[i];

			//Our current joint...
			Joint& joint = skeleton[jointLookup[j_id]];

			//Which node represents our joint in the glTF file...
			const tinygltf::Node& node = gltf.nodes[j_id];

			//node.children contains glTF IDs for the joints
			//that should be "under" our current joint in the hierarchy.
			for (size_t j = 0; j < node.children.size(); ++j)
			{
				auto it = jointLookup.find(node.children[j]);

				if (it != jointLookup.end())
				{
					//Set up ourselves as a parent of the current child node.
					skeleton[it->second].m_parent = true;
					skeleton[it->second].m_parentInd = jointLookup[j_id];
					//Add the current child node to our list of children.
					joint.m_childrenInd.push_back(it->second);
				}
			}
		}

		skeleton.DoFK();
		return true;
	}

	bool ExtractJointLookup(const tinygltf::Model& gltf,
							JointIndexLookup& jointLookup,
							std::string& err, std::string& warn)
	{
		if (gltf.skins.size() == 0)
		{
			err = "No skeleton data in file.";
			return false;
		}

		const tinygltf::Skin& skin = gltf.skins[0];

		if (skin.joints.size() == 0)
		{
			err = "No joints specified in skeleton.";
			return false;
		}

		for (size_t i = 0; i < skin.joints.size(); ++i)
		{
			int j_id = skin.joints[i];
			jointLookup[j_id] = static_cast<int>(i);
		}

		return true;
	}

	bool ExtractSkinWeights(const tinygltf::Model& gltf, SkinnedMesh& mesh, 
						    std::string& err, std::string& warn)
	{
		if (gltf.meshes.size() == 0 || gltf.meshes[0].primitives.size() == 0)
		{
			err = "Cannot load joint influences and weights - no geometry data in file.";
			return false;
		}

		const tinygltf::Primitive& geom = gltf.meshes[0].primitives[0];

		int influenceID = FindAccessor(geom, "JOINTS_0");
		int weightID = FindAccessor(geom, "WEIGHTS_0");

		if (influenceID == -1)
		{
			err = "No joint influence data present in file.";
			return false;
		}

		if (weightID == -1)
		{
			err = "No skin weight data present in file.";
			return false;
		}

		DataGetter infGetter = BuildGetter(gltf, influenceID);
		DataGetter wtGetter = BuildGetter(gltf, weightID);

		DataGetter faceIndexer = BuildGetter(gltf, geom.indices);

		if (faceIndexer.elementSize != sizeof(GLshort))
		{
			err = "Primitive indices are in a currently unsupported format." \
				"Consider changing your GLTF export settings, or else check for " \
				"and support this format in your GLTF loader implementation.";

			return false;
		}

		if (infGetter.elementSize != 4 * sizeof(GLushort))
		{
			err = "Joint influences are in a currently unsupported format." \
				  "Consider changing your GLTF export settings, or else check for " \
				  "and support this format in your GLTF loader implementation.";

			return false;
		}

		if (wtGetter.elementSize != 4 * sizeof(GLfloat))
		{
			err = "Skin weights are in a currently unsupported format." \
				  "Consider changing your GLTF export settings, or else check for " \
				  "and support this format in your GLTF loader implementation.";

			return false;
		}

		std::vector<glm::vec4> influences;
		std::vector<glm::vec4> weights;

		influences.resize(faceIndexer.len);
		weights.resize(faceIndexer.len);

		for (size_t i = 0; i < faceIndexer.len; ++i)
		{
			GLushort j0, j1, j2, j3;

			GLshort faceIndex;
			memcpy(&faceIndex, &faceIndexer.data[i * faceIndexer.stride], sizeof(GLshort));

			size_t face = faceIndex;

			memcpy(&j0, &infGetter.data[face * infGetter.stride], sizeof(GLushort));
			memcpy(&j1, &infGetter.data[face * infGetter.stride + sizeof(GLushort)], sizeof(GLushort));
			memcpy(&j2, &infGetter.data[face * infGetter.stride + 2 * sizeof(GLushort)], sizeof(GLushort));
			memcpy(&j3, &infGetter.data[face * infGetter.stride + 3 * sizeof(GLushort)], sizeof(GLushort));
		
			//Note: glTF appears to "switch" between how joints are indexed
			//when it comes to skin weights - the joint IDs will match
			//the ORDER of nodes in the skeleton definition, rather than the
			//IDENTIFIERS of those nodes. This means we no longer need
			//to go through our joint lookup to convert the indices - 
			//they're already following the same convention we use in NOU.
			influences[i] = glm::vec4(j0, j1, j2, j3);

			memcpy(&weights[i], &wtGetter.data[face * wtGetter.stride], sizeof(glm::vec4));
		}

		mesh.SetJointInfluences(influences);
		mesh.SetSkinWeights(weights);

		return true;
	}

	bool ExtractSkeletalAnimation(const tinygltf::Model& gltf, SkeletalAnim& anim,
								  const JointIndexLookup& jointLookup,
								  std::string& err, std::string& warn)
	{
		if (gltf.animations.size() == 0)
		{
			err = "No skeletal animation data in file.";
			return false;
		}

		const tinygltf::Animation& gltfAnim = gltf.animations[0];

		//TimeGetter will grab the timestamp of a given keyframe.
		//KeyGetter will grab the actual keyframe data.
		DataGetter timeGetter, keyGetter;

		float maxTime = 0.0f;

		JointAnimLookup animLookup = {};

		anim.data.reserve(gltfAnim.channels.size());

		for (size_t i = 0; i < gltfAnim.channels.size(); ++i)
		{
			const tinygltf::AnimationChannel& channel = gltfAnim.channels[i];

			ChannelType channelType = ChannelType::NONE;

			//We only care about joint position/rotation.
			if (channel.target_path == "rotation")
				channelType = ChannelType::ROTATION;
			else if (channel.target_path == "translation")
				channelType = ChannelType::POSITION;

			if (channelType == ChannelType::NONE)
				continue;
			
			auto jointIt = jointLookup.find(channel.target_node);

			if (jointIt == jointLookup.end())
			{
				warn += "\nFound joint animation for node not in hierarchy with ID: "
						+ std::to_string(channel.target_node);

				continue;
			}

			const tinygltf::AnimationSampler& sampler = gltfAnim.samplers[channel.sampler];

			timeGetter = BuildGetter(gltf, sampler.input);
			keyGetter = BuildGetter(gltf, sampler.output);

			if (keyGetter.len == 0 || timeGetter.len == 0)
				continue;
			
			int jointInd = jointIt->second;

			//If we've already created an animation for this joint
			//(e.g., we already got rotation and now need the position channel),
			//then grab a reference to our existing animation.
			auto animIt = animLookup.find(jointInd);
			JointAnim* data = nullptr;

			//If we haven't already made an animation for this joint,
			//make a new one and add it to our registry of animation data.
			if (animIt == animLookup.end())
			{
				anim.data.push_back(JointAnim());
				data = &(anim.data[anim.data.size() - 1]);

				animLookup.insert({jointInd, data});	
			}
			else
				data = animIt->second;

			data->jointInd = jointInd;

			//Normally we'd use templates to keep this sort of thing cleaned up.
			//Unfortunately, that whole difference in quaternion layout throws a
			//wrinkle into that - we can't just memcpy into a raw glm::quat.
			//Rather than correct after the fact and swap X/W coordinates,
			//we'll just do this.
			switch (channelType)
			{
				//Extract frame time as a float and position keyframes as vec3s.
				case ChannelType::POSITION:

				data->posFrames = static_cast<int>(keyGetter.len);
				data->posKeys.resize(keyGetter.len);
				data->posTimes.resize(keyGetter.len);
				
				for (size_t k = 0; k < timeGetter.len && k < keyGetter.len; ++k)
				{
					float fTime;
					glm::vec3 pos;

					memcpy(&fTime, &timeGetter.data[k * timeGetter.stride], sizeof(float));

					if (fTime > maxTime)
						maxTime = fTime;

					memcpy(&pos, &keyGetter.data[k * keyGetter.stride], sizeof(glm::vec3));

					data->posKeys[k] = pos;
					data->posTimes[k] = fTime;
				}

				break;

				//Extract frame time as a float and rotation keyframes as quats.
				case ChannelType::ROTATION:

				data->rotFrames = static_cast<int>(keyGetter.len);
				data->rotKeys.resize(keyGetter.len);
				data->rotTimes.resize(keyGetter.len);

				for (size_t k = 0; k < timeGetter.len && k < keyGetter.len; ++k)
				{
					float fTime;
					float x, y, z, w;

					memcpy(&fTime, &timeGetter.data[k * timeGetter.stride], sizeof(float));

					if (fTime > maxTime)
						maxTime = fTime;

					memcpy(&x, &keyGetter.data[k * keyGetter.stride],
						sizeof(float));

					memcpy(&y, &keyGetter.data[k * keyGetter.stride + sizeof(float)],
						sizeof(float));

					memcpy(&z, &keyGetter.data[k * keyGetter.stride + 2 * sizeof(float)],
						sizeof(float));

					memcpy(&w, &keyGetter.data[k * keyGetter.stride + 3 * sizeof(float)],
						sizeof(float));

					data->rotKeys[k] = glm::normalize(glm::quat(w, x, y, z));
					data->rotTimes[k] = fTime;
				}

				break;

				default:
				break;
			}
		}

		anim.duration = maxTime;
		return true;
	}
}