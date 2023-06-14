#include "TextureLoader.hpp"
#include "GL/glew.h"
#include <iostream>
#include <stb_image/stb_image.h>
#include "../util/ErrorHandling.hpp"

unsigned int TextureLoader::LoadTexture(std::string_view p_Path, unsigned int p_ScaleMode, unsigned int p_WrapMode) {
	int t_Width, t_Height, t_BPP;

	stbi_set_flip_vertically_on_load(1);
	unsigned char* t_LocalBuffer = stbi_load(p_Path.data(), &t_Width, &t_Height, &t_BPP, 4);

	unsigned int t_TextureID;
	GLCall(glGenTextures(1, &t_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, t_TextureID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_WrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_WrapMode));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, t_Width, t_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (t_LocalBuffer) {
		stbi_image_free(t_LocalBuffer);
	}

    return t_TextureID;
}