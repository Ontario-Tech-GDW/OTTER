//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This header contains a Glyph renderer for rendering TrueType fonts in
// OpenGL. This was originally based off of the Glyph Renderer from the
// Sprout engine
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "GLM/glm.hpp"
#include "glad/glad.h"
#include "stb_truetype.h"

namespace  TTK
{
	struct GlyphInfo {
		glm::vec2 Positions[4];
		glm::vec2 UVs[4];
		float OffsetX, OffsetY;
	};

	/* TODO: Font alignment
	enum HorizAlign {
		Left = 0,
		Justified = 1,
		Right = 2
	};

	enum VertAlign {
		Top = 0,
		Middle = 1,
		Bottom = 2
	};
	*/

	struct Col8 {
		char R, G, B, A;
	};

	class FontRenderer;
	
	class TrueTypeTextureFont {
	public:
		TrueTypeTextureFont(const char* fileName, uint32_t size);
		~TrueTypeTextureFont();
		
		GlyphInfo GetGlyph(int codePoint, float offsetX, float offsetY) const;
		float  GetKerning(int char1, int char2) const;
		float  GetLineHeight() const;

		virtual glm::vec2 MeausureString(const char* text, const float scale = 1.0f);

		virtual GLint GetTexture() const { return myTexture; }

	protected:
		friend class FontRenderer;
		GLuint   myTexture;
		GLuint64 m_TexHandle;

		const uint32_t ATLAS_WIDTH = 1024;
		const uint32_t ATLAS_HEIGHT = 1024;
		const uint32_t FONT_OVERSAMPLE_X = 2;
		const uint32_t FONT_OVERSAMPLE_Y = 2;
		const uint32_t FIRST_CHAR = ' ';
		const uint32_t CHAR_COUNT = '~' - ' ';

		stbtt_packedchar* myCharInfo;
		uint32_t          myFontSize;
		stbtt_fontinfo    myFontInfo;
		float             myPixelHeightScale;
		float             myEmToPixel;
		int               myAscent,
						  myDescent,
						  myLineGap;
	};
	
	class FontRenderer {
	public:
		static FontRenderer& Instance() {
			if (m_Instance == nullptr)
				m_Instance = new FontRenderer();
			return *m_Instance;
		}
		static void DestroyContext() {
			delete m_Instance;
			m_Instance = nullptr;
		}

	private:
		static FontRenderer* m_Instance;

		struct Vert {
			glm::vec2 Position;
			Col8      Color;
			glm::vec2 UV;
		};

	public:
		~FontRenderer();

		void Render(const TrueTypeTextureFont& font, const char* text, const glm::vec2& pos, const glm::vec4& color, float scale = 1.0f);
		
	private:
		FontRenderer();
				
		GLuint   m_ShaderHandle;
		GLuint   m_VAO, m_VBO, m_EBO;
		Vert     m_MeshData[256 * 4];
		GLuint   m_IndexData[256 * 6];
	};
}