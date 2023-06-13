#include "Texture2D.hpp"
#include "TextureLoader.hpp"
#include "../util/ErrorHandling.hpp" 

Texture2D::Texture2D(std::string_view p_Path, unsigned int p_ScaleMode, unsigned int p_WrapMode) 
	: m_TextureID(TextureLoader::LoadTexture(p_Path, p_ScaleMode, p_WrapMode)) {}

void Texture2D::Bind(unsigned int p_Slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + p_Slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture2D::Unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}