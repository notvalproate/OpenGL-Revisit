#pragma once
#include "GL/glew.h"
#include <string_view>

class Texture2D {
public:
	Texture2D(std::string_view p_Path, unsigned int p_ScaleMode = GL_LINEAR, unsigned int p_WrapMode = GL_REPEAT);

	void Bind(unsigned int p_Slot = 0);
	void Unbind();

	[[nodiscard]] unsigned int GetID() const { return m_TextureID; }
private:
	unsigned int m_TextureID;
};