#include "Texture2D.hpp"
#include "TextureLoader.hpp"
#include "../util/ErrorHandling.hpp" 

Texture2D::Texture2D(std::string_view path, TextureType type, unsigned int scaleMode, unsigned int wrapMode) 
	: m_TextureID(textureloader::loadTexture(path, scaleMode, wrapMode)), m_Type(type) {}

void Texture2D::bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture2D::unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}