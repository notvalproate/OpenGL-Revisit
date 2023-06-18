#include "Texture2D.hpp"
#include "TextureLoader.hpp"
#include "../util/ErrorHandling.hpp" 

Texture2D::Texture2D(std::string_view path, unsigned int scaleMode, unsigned int wrapMode) 
	: m_TextureID(textureloader::loadTexture(path, scaleMode, wrapMode)) {}

void Texture2D::bind(unsigned int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture2D::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}