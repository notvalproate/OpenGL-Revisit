#pragma once
#include "GL/glew.h"
#include <string_view>

enum class TextureType : unsigned short {
	DIFFUSE = 0x00,
	SPECULAR = 0x01,
	NORMAL = 0x02
};

class Texture2D {
public:
	Texture2D(std::string_view path, TextureType type, unsigned int scaleMode = GL_LINEAR, unsigned int wrapMode = GL_REPEAT);

	void bind(unsigned int Slot = 0) const;
	void unbind() const;

	[[nodiscard]] unsigned int getID() const { return m_TextureID; }
	[[nodiscard]] TextureType getType() const { return m_Type; }
private:
	unsigned int m_TextureID;
	TextureType m_Type;
};