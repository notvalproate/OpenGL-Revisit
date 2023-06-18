#pragma once
#include "GL/glew.h"
#include <string_view>

class Texture2D {
public:
	Texture2D(std::string_view Path, unsigned int ScaleMode = GL_LINEAR, unsigned int WrapMode = GL_REPEAT);

	void bind(unsigned int Slot = 0);
	void unbind();

	[[nodiscard]] unsigned int getID() const { return m_TextureID; }
private:
	unsigned int m_TextureID;
};