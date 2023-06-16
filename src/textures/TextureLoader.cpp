#include "TextureLoader.hpp"
#include "GL/glew.h"
#include <iostream>
#include <stb_image/stb_image.h>
#include "../util/ErrorHandling.hpp"

unsigned int textureloader::loadTexture(std::string_view a_Path, unsigned int a_ScaleMode, unsigned int a_WrapMode) {
	int t_Width, t_Height, t_BPP;

	stbi_set_flip_vertically_on_load(1);
	unsigned char* t_LocalBuffer = stbi_load(a_Path.data(), &t_Width, &t_Height, &t_BPP, 4);

	unsigned int t_TextureID;
	GLCall(glGenTextures(1, &t_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, t_TextureID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_WrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_WrapMode));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, t_Width, t_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (t_LocalBuffer) {
		stbi_image_free(t_LocalBuffer);
	}

    return t_TextureID;
}