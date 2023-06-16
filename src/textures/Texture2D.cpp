#include "Texture2D.hpp"
#include "TextureLoader.hpp"
#include "../util/ErrorHandling.hpp" 

Texture2D::Texture2D(std::string_view a_Path, unsigned int a_ScaleMode, unsigned int a_WrapMode) 
	: m_TextureID(textureloader::loadTexture(a_Path, a_ScaleMode, a_WrapMode)) {}

void Texture2D::bind(unsigned int a_Slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + a_Slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture2D::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}