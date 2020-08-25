// PUT YOUR NAME AND STUDENT NUMBER HERE //

#include "TTK/SpriteSheetQuad.h"
#include <iostream>

#include <glad/glad.h>
#include "Logging.h"

TTK::SpriteSheetQuad::SpriteSheetQuad()
{
	m_Vertices[0].Position = { -1.0f,  1.0f, 0.0f };
	m_Vertices[1].Position = {  1.0f,  1.0f, 0.0f };
	m_Vertices[2].Position = { -1.0f, -1.0f, 0.0f };	
	m_Vertices[3].Position = {  1.0f, -1.0f, 0.0f };

	m_DoesLoop = true;
	m_CurrentFrame = 0;
	m_FrameTime = 0;
	m_Color = glm::vec4(1.0f);
	m_FrameLength = std::vector<float>();
	m_SpriteCoordinates = std::vector<SpriteCoordinates>();
	m_Texture = TTK::Texture2D();

	uint32_t indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	int currentVAO = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVert) * 4, m_Vertices, GL_STREAM_DRAW);
	glCreateBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);
	#pragma warning(push)
	#pragma warning(disable: 6011)
	QuadVert* nullVert = nullptr;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(QuadVert), &(nullVert->Position));
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(QuadVert), &(nullVert->Texture));
	glBindVertexArray(currentVAO);
	#pragma warning(pop)

	const char* vsSource = R"LIT(#version 440
            layout (location = 0) in vec3 vertexPosition;
            layout (location = 1) in vec2 vertexTexture;
            layout (location = 0) out vec2 fragmentTexture;
            layout (location = 0) uniform mat4 xTransform;	
            void main() {
                gl_Position = xTransform * vec4(vertexPosition, 1);
                fragmentTexture = vertexTexture;
            })LIT";

	const char* fsSource = R"LIT(#version 440
            layout(binding = 0) uniform sampler2D xSampler;
            layout(location = 2) uniform vec4 xColor;
            layout (location = 0) in vec2 fragUv;            	
            out vec4 frag_color;            	
            void main() {
				frag_color = texture2D(xSampler, fragUv) * xColor;
            })LIT";

	m_Shader = glCreateProgram();

	GLuint programs[2];
	programs[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(programs[0], 1, &vsSource, NULL);
	glCompileShader(programs[0]);
	programs[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(programs[1], 1, &fsSource, NULL);
	glCompileShader(programs[1]);

	// Attach our two shaders
	glAttachShader(m_Shader, programs[0]);
	glAttachShader(m_Shader, programs[1]);

	// Perform linking
	glLinkProgram(m_Shader);

	// Remove shader parts to save space
	glDetachShader(m_Shader, programs[0]);
	glDeleteShader(programs[0]);
	glDetachShader(m_Shader, programs[1]);
	glDeleteShader(programs[1]);
}

void TTK::SpriteSheetQuad::SliceSpriteSheet(const char* fileName, float spriteSizeX, float spriteSizeY,
	int numSpritesPerRow, int numRows, float animTime) {
	SliceSpriteSheet(fileName, numSpritesPerRow, numRows, animTime);
}

void TTK::SpriteSheetQuad::SliceSpriteSheet(const char* fileName, int numSpritesPerRow, int numRows, float animTime)
{
	m_Texture.LoadTextureFromFile(fileName);

	float spriteWidth = static_cast<float>(m_Texture.GetWidth()) / numSpritesPerRow;
	float spriteHeight = static_cast<float>(m_Texture.GetHeight()) / numRows;

	float frameTime = animTime / (numSpritesPerRow * numRows);

	if (animTime == 0.0f)
		frameTime = 1.0f / 60.0f;

	for (int j = 0; j < numRows; j++) // loop through each row
	{
		for (int i = 0; i < numSpritesPerRow; i++) // loop through each sprite in the row
		{
			SpriteCoordinates sc;

			// calculates the pixel coordinates
			sc.xMin = i * spriteWidth;
			sc.xMax = sc.xMin + spriteWidth;

			sc.yMin = j * spriteHeight;
			sc.yMax = sc.yMin + spriteHeight;

			// calculate the normalized coordinates
			sc.uMin = sc.xMin / m_Texture.GetWidth();
			sc.uMax = sc.xMax / m_Texture.GetWidth();

			sc.vMin = sc.yMin / m_Texture.GetHeight();
			sc.vMax = sc.yMax / m_Texture.GetHeight();

			m_SpriteCoordinates.push_back(sc);
			m_FrameLength.push_back(frameTime);
		}
	}
}


void TTK::SpriteSheetQuad::Update(float deltaTime) {
	// Advance the frame time by how much time has passed since the last draw call
	m_FrameTime += deltaTime;

	if (m_FrameTime > m_FrameLength[m_CurrentFrame]) {
		m_FrameTime -= m_FrameLength[m_CurrentFrame];
		m_CurrentFrame++;
		if (m_DoesLoop)
			m_CurrentFrame = m_CurrentFrame % static_cast<int>(m_FrameLength.size());
		else
			m_CurrentFrame = m_CurrentFrame > static_cast<int>(m_FrameLength.size()) - 1 ? static_cast<int>(m_FrameLength.size()) - 1 : m_CurrentFrame;
	}
}

void TTK::SpriteSheetQuad::ResetAnimation() {
	m_CurrentFrame = 0;
	m_FrameTime = 0;
}

void TTK::SpriteSheetQuad::SetLooping(bool loop) {
	m_DoesLoop = loop;
}

void TTK::SpriteSheetQuad::Draw(const glm::mat4& matrix)
{
	SpriteCoordinates sc = m_SpriteCoordinates[m_CurrentFrame];

	// set the UVs for the quad
	// quad.textureCoordinates //messy
	m_Vertices[0].Texture = { sc.uMin, sc.vMin };
	m_Vertices[1].Texture = { sc.uMax, sc.vMin };
	m_Vertices[2].Texture = { sc.uMin, sc.vMax };
	m_Vertices[3].Texture = { sc.uMax, sc.vMax };
	
	int currentProgram, currentVAO;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
	glUseProgram(m_Shader);
	glProgramUniform4fv(m_Shader, 2, 1, &m_Color.x);
	glProgramUniformMatrix4fv(m_Shader, 0, 1, false, &matrix[0][0]);
	m_Texture.Bind();
	glBindVertexArray(m_VAO);
	glNamedBufferData(m_VBO, sizeof(QuadVert) * 4, m_Vertices, GL_STREAM_DRAW);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	m_Texture.Unbind();
	glBindVertexArray(currentVAO);
	glUseProgram(currentProgram);
}

void TTK::SpriteSheetQuad::SetFrameLength(int frameNumber, float time)
{
	if (frameNumber >= 0 && frameNumber < m_FrameLength.size()) {
		m_FrameLength[frameNumber] = time;
	}
	else {
		LOG_ERROR("SpriteSheetQuad.cpp Error! Frame {} does not exist!", frameNumber);
	}
}

void TTK::SpriteSheetQuad::SetFrameLengths(const std::vector<float>& time) {
	LOG_ASSERT(time.size() == m_FrameLength.size(), "SpriteSheetQuad.cpp Error! Vector length mismatch!");
	m_FrameLength = time;
}

const std::vector<float>& TTK::SpriteSheetQuad::GetFrameLengths() const {
	return m_FrameLength;
}

int TTK::SpriteSheetQuad::GetNumberOfFrames() const {
	return static_cast<int>(m_SpriteCoordinates.size());
}

float TTK::SpriteSheetQuad::GetFrameLength(int frameNumber) const {
	return m_FrameLength[frameNumber];
}
