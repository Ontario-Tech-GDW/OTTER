/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Texture.h
Simple wrapper for STB image loading into 2D textures.
*/

#pragma once

#include "glad/glad.h"

#include <string>

namespace nou
{
	class Texture2D
	{
		public:

		Texture2D(const std::string& filename);
		~Texture2D();

		GLuint GetID() const;

		private:

		GLuint m_id;
	};
}