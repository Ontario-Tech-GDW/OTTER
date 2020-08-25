//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file implements the TTK glyph renderer
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#include "TTK/FontRenderer.h"
#include <fstream>
#include "Logging.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "TTK/TTKContext.h"

// Implementaiton of readFile
char* readFile(const char* filename) {
	// Declare and open the file stream
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// Only read if the file is open
	if (file.is_open()) {
		// Get the starting location in the file
		size_t fileSize = file.tellg();
		// Seek to the end
		file.seekg(0, std::ios::end);
		// Calculate the file size from end to beginning
		fileSize = (size_t)file.tellg() - fileSize;
		// Seek back to the beginning of the file
		file.seekg(0, std::ios::beg);

		char* result = new char[fileSize + 1];
		// Read the entire file to our memory
		file.read(result, fileSize);

		// Make our text null-terminated
		result[fileSize] = '\0';

		// Close the file before returning
		file.close();
		return result;

	}
	// Otherwise, we failed to open our file, throw a runtime error
	else {
		return nullptr;
	}
}

TTK::FontRenderer* TTK::FontRenderer::m_Instance = nullptr;

TTK::TrueTypeTextureFont::TrueTypeTextureFont(const char* fileName, uint32_t size)
{
	myFontSize = size;

	unsigned char* fontData = (unsigned char*)readFile(fileName);
	uint8_t* atlasData = new uint8_t[static_cast<size_t>(ATLAS_WIDTH) * ATLAS_HEIGHT];

	myCharInfo = new stbtt_packedchar[CHAR_COUNT];

	if (!stbtt_InitFont(&myFontInfo, fontData, 0)) {
		LOG_ERROR("Failed to initialize font");
		delete[] atlasData;
		delete[] fontData;
		return;
	}

	// Gets the font metrics
	stbtt_GetFontVMetrics(&myFontInfo, &myAscent, &myDescent, &myLineGap);

	myPixelHeightScale = stbtt_ScaleForPixelHeight(&myFontInfo, static_cast<float>(size));
	myEmToPixel = stbtt_ScaleForMappingEmToPixels(&myFontInfo, 1.0f);

	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, atlasData, ATLAS_WIDTH, ATLAS_HEIGHT, 0, 1, nullptr)) {
		LOG_ERROR("Failed to pack font texture");
		delete[] atlasData;
		delete[] fontData;
		return;
	}

	stbtt_PackSetOversampling(&context, FONT_OVERSAMPLE_X, FONT_OVERSAMPLE_Y);
	if (!stbtt_PackFontRange(&context, fontData, 0, static_cast<float>(size), FIRST_CHAR, CHAR_COUNT, myCharInfo)) {
		LOG_ERROR("Failed to pack font range");
		delete[] atlasData;
		delete[] fontData;
		return;
	}
	stbtt_PackEnd(&context);

	for (uint32_t ix = FIRST_CHAR; ix < FIRST_CHAR + CHAR_COUNT; ix++) {
		// TODO: Grab glyph data
	}

	// Create and upload the texture to store our font in
	LOG_ASSERT(glGetError() == GL_NONE, "Some error has occured!");
	glCreateTextures(GL_TEXTURE_2D, 1, &myTexture);
	glTextureParameteri(myTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(myTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(myTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTextureParameteri(myTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOG_ASSERT(glGetError() == GL_NONE, "Some error has occured!");
	glTextureStorage2D(myTexture, 1, GL_R8, ATLAS_WIDTH, ATLAS_HEIGHT);
	LOG_ASSERT(glGetError() == GL_NONE, "Internal texture format not supported");
	glTextureSubImage2D(myTexture, 0, 0, 0, ATLAS_WIDTH, ATLAS_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, atlasData);
	LOG_ASSERT(glGetError() == GL_NONE, "Texture transfer format not supported");
	m_TexHandle = glGetTextureHandleARB(myTexture);
	glMakeTextureHandleResidentARB(m_TexHandle);

	delete[] atlasData;
	delete[] fontData;
}

TTK::TrueTypeTextureFont::~TrueTypeTextureFont()
{
	delete[] myCharInfo;
	glDeleteTextures(1, &myTexture);
}

TTK::GlyphInfo TTK::TrueTypeTextureFont::GetGlyph(int codePoint, float offsetX, float offsetY) const {
	stbtt_aligned_quad quad;

	stbtt_GetPackedQuad(myCharInfo, ATLAS_WIDTH, ATLAS_HEIGHT, codePoint - FIRST_CHAR, &offsetX, &offsetY, &quad, 1);
	auto xmin = quad.x0;
	auto xmax = quad.x1;
	auto ymin = -quad.y1;
	auto ymax = -quad.y0;

	GlyphInfo info = GlyphInfo();
	info.OffsetX = offsetX;
	info.OffsetY = offsetY;
	info.Positions[0] = { xmax, -ymin };
	info.Positions[1] = { xmax, -ymax };
	info.Positions[2] = { xmin, -ymax };
	info.Positions[3] = { xmin, -ymin };
	info.UVs[0] = { quad.s1, quad.t1 };
	info.UVs[1] = { quad.s1, quad.t0 };
	info.UVs[2] = { quad.s0, quad.t0 };
	info.UVs[3] = { quad.s0, quad.t1 };

	return info;
}

float TTK::TrueTypeTextureFont::GetKerning(int char1, int char2) const {
	return stbtt_GetCodepointKernAdvance(&myFontInfo, char1, char2) * myPixelHeightScale;
}

float TTK::TrueTypeTextureFont::GetLineHeight() const {
	return (myAscent - myDescent + myLineGap) * myPixelHeightScale;
}

glm::vec2 TTK::TrueTypeTextureFont::MeausureString(const char* text, const float scale) {
	float multiplier = scale;

	glm::vec2 originPos = glm::vec2();

	GlyphInfo glyph;

	size_t length = strlen(text);
	float xOff{ 0 }, yOff{ 0 };
	float lineHeight = 0.0f;
	float maxWidth = 0.0f;
	float totalHeight = 0.0f;

	for (size_t i = 0; i < length; i++) {
		glyph = GetGlyph(text[i], xOff, yOff);
		xOff = glyph.OffsetX;
		yOff = glyph.OffsetY;

		lineHeight = glm::max(lineHeight, -glyph.Positions[1].y);
		maxWidth = glm::max(maxWidth, xOff);

		if (text[i] == '\n')
		{
			yOff += GetLineHeight() * multiplier;
			totalHeight += lineHeight;
			lineHeight = 0.0f;
			xOff = 0;
		}
		else if (text[i] == '\r') {
			xOff = 0;
		}
		else if (text[i] == '\t') {
			float xOffTemp{ 0 }, yOffTemp{ 0 };
			glyph = GetGlyph(' ', xOffTemp, yOffTemp);
			xOff += glyph.OffsetX * 4;
		}
	}
	GlyphInfo space = GetGlyph('|', xOff, yOff);
	yOff += -(space.Positions[1].y - space.Positions[0].y);
	return glm::vec2(xOff, yOff);
}

TTK::FontRenderer::~FontRenderer()
{
	glDeleteShader(m_ShaderHandle);
	glDeleteVertexArrays(1, &m_VAO);
}

void TTK::FontRenderer::Render(const TrueTypeTextureFont& font, const char* text, const glm::vec2& pos, const glm::vec4& color, float scale)
{
	size_t length = strlen(text);
	
	float multiplier = scale;

	// The number of quads ACTUALLY drawn Shit-For-Brains
	int quads = 0;

	glm::vec2 originPos = glm::vec2(pos.x, pos.y);

	static GlyphInfo glyph;

	Col8 gpuCol;
	gpuCol.R = static_cast<char>(color.r * 255);
	gpuCol.G = static_cast<char>(color.g * 255);
	gpuCol.B = static_cast<char>(color.b * 255);
	gpuCol.A = static_cast<char>(color.a * 255);

	float xOff{ 0 }, yOff{ 0 };

	for (int i = 0; i < length; i++) {
		glyph = font.GetGlyph(text[i], xOff, yOff);
		xOff = glyph.OffsetX;
		yOff = glyph.OffsetY;

		if (text[i] == '\n')
		{
			yOff += font.GetLineHeight() * multiplier;
			xOff = 0;
		}
		else if (text[i] == '\r') {
			xOff = 0;
		}
		else if (text[i] == '\t') {
			float xOffTemp{ 0 }, yOffTemp{ 0 };
			glyph = font.GetGlyph(' ', xOffTemp, yOffTemp);
			xOff += glyph.OffsetX * 4;
		}
		else {
			m_MeshData[quads * 4 + 0].Position = originPos + glyph.Positions[0] * multiplier;
			m_MeshData[quads * 4 + 0].UV = glyph.UVs[0];
			m_MeshData[quads * 4 + 0].Color = gpuCol;

			m_MeshData[quads * 4 + 1].Position = originPos + glyph.Positions[1] * multiplier;
			m_MeshData[quads * 4 + 1].UV = glyph.UVs[1];
			m_MeshData[quads * 4 + 1].Color = gpuCol;

			m_MeshData[quads * 4 + 2].Position = originPos + glyph.Positions[2] * multiplier;
			m_MeshData[quads * 4 + 2].UV = glyph.UVs[2];
			m_MeshData[quads * 4 + 2].Color = gpuCol;

			m_MeshData[quads * 4 + 3].Position = originPos + glyph.Positions[3] * multiplier;
			m_MeshData[quads * 4 + 3].UV = glyph.UVs[3];
			m_MeshData[quads * 4 + 3].Color = gpuCol;


			m_IndexData[quads * 6 + 0] = quads * 4 + 0;
			m_IndexData[quads * 6 + 1] = quads * 4 + 1;
			m_IndexData[quads * 6 + 2] = quads * 4 + 2;

			m_IndexData[quads * 6 + 3] = quads * 4 + 0;
			m_IndexData[quads * 6 + 4] = quads * 4 + 2;
			m_IndexData[quads * 6 + 5] = quads * 4 + 3;

			quads++;
		}
	}

	length = quads;

	// Update and render our meshes
	bool blendState = glIsEnabled(GL_BLEND);
	GLboolean depthMaskEnabled = false;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glGetError();
	glm::mat4 proj = TTK::Context::Instance().GetOrthoProjection();
	glUseProgram(m_ShaderHandle);
	glProgramUniformMatrix4fv(m_ShaderHandle, 0, 1, false, &proj[0][0]);
	glProgramUniformHandleui64ARB(m_ShaderHandle, 1, font.m_TexHandle);	
	glBindVertexArray(m_VAO);
	glNamedBufferSubData(m_VBO, 0, length * 4 * sizeof(Vert), m_MeshData);
	glNamedBufferSubData(m_EBO, 0, length * 6 * sizeof(GLuint), m_IndexData);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(length * 6), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	LOG_ASSERT(glGetError() == GL_NONE, "Failed to draw our text mesh!");
	if (!blendState) glDisable(GL_BLEND);
	glDepthMask(depthMaskEnabled);
}

TTK::FontRenderer::FontRenderer() {
	LOG_INFO("Initializing font renderer");

	memset(m_MeshData, 0, sizeof(m_MeshData));
	memset(m_IndexData, 0, sizeof(m_IndexData));

	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	GLuint buffers[2];
	glCreateBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glNamedBufferData(buffers[0], 256 * 4 * sizeof(Vert), m_MeshData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glNamedBufferData(buffers[1], 256 * 6 * sizeof(GLuint), m_IndexData, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	#pragma warning(push)
	#pragma warning(disable: 6011)
	Vert* v = nullptr;
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vert), &v->Position);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, false, sizeof(Vert), &v->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vert), &v->UV);
	#pragma warning(pop)
	
	glBindVertexArray(0);

	m_VBO = buffers[0];
	m_EBO = buffers[1];

	const char* vsSource = R"LIT(#version 430
            layout (location = 0) in vec2 vertexPosition;
            layout (location = 1) in vec4 vertexColor;
            layout (location = 2) in vec2 vertexTexture;	
            layout (location = 0) out vec4 fragmentColor;
            layout (location = 1) out vec2 fragmentTexture;
            layout (location = 0) uniform mat4 xTransform;	
            void main() {
                gl_Position = xTransform * vec4(vertexPosition, 0, 1);
                fragmentColor = vertexColor;
                fragmentTexture = vertexTexture;
            })LIT";

	const char* fsSource = R"LIT(#version 430
			#extension GL_ARB_bindless_texture : enable
            layout(bindless_sampler, location = 1) uniform sampler2D xSampler;
            layout (location = 0) in vec4 fragColor;
            layout (location = 1) in vec2 fragUv;            	
            out vec4 frag_color;            	
            void main() {
                frag_color = fragColor;
				frag_color.a = texture2D(xSampler, fragUv).r;
            })LIT";

	m_ShaderHandle = glCreateProgram();

	GLuint programs[2];
	programs[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(programs[0], 1, &vsSource, NULL);
	glCompileShader(programs[0]);
	programs[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(programs[1], 1, &fsSource, NULL);
	glCompileShader(programs[1]);
	
	// Attach our two shaders
	glAttachShader(m_ShaderHandle, programs[0]);
	glAttachShader(m_ShaderHandle, programs[1]);

	// Perform linking
	glLinkProgram(m_ShaderHandle);

	// Remove shader parts to save space
	glDetachShader(m_ShaderHandle, programs[0]);
	glDeleteShader(programs[0]);
	glDetachShader(m_ShaderHandle, programs[1]);
	glDeleteShader(programs[1]);

	glBindVertexArray(0);
	
	LOG_INFO("Done initilaizing font renderer");
}
