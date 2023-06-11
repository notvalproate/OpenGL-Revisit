#include "Texture2D.hpp"
#include <iostream>
#include <stb_image/stb_image.h>
#include "ErrorHandling.hpp"

Texture2D::Texture2D(std::string_view p_Path, unsigned int p_ScaleMode, unsigned int p_WrapMode) {
	int t_Width, t_Height, t_Channels;

	unsigned char* t_LocalBuffer = stbi_load(p_Path.data(), &t_Width, &t_Height, &t_Channels, 0); 

	GLCall(glGenTextures(1, &m_TextureID));
	Bind();

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_ScaleMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_WrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_WrapMode));

	if (t_LocalBuffer) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t_Width, t_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "Failed to load texture. Path: " << p_Path << std::endl;
	}

	stbi_image_free(t_LocalBuffer);
	Unbind(); 
}

void Texture2D::Bind(unsigned int p_Slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + p_Slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture2D::Unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}