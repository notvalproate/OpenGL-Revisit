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
	Texture2D(std::string_view path, unsigned int scaleMode = GL_LINEAR, unsigned int wrapMode = GL_REPEAT);

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	[[nodiscard]] std::string getPath() const { return m_Path; }
private:
	unsigned int m_TextureID;
	std::string m_Path;
};