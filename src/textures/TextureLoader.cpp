#include "TextureLoader.hpp"
#include "GL/glew.h"
#include <iostream>
#include <stb_image/stb_image.h>
#include "../util/ErrorHandling.hpp"

namespace textureloader {
	unsigned int loadTexture(std::string_view path, unsigned int scaleMode, unsigned int wrapMode) {
		int width, height, bpp;

		stbi_set_flip_vertically_on_load(1);
		unsigned char* localBuffer = stbi_load(path.data(), &width, &height, &bpp, 4);

		unsigned int textureID;
		GLCall(glGenTextures(1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (localBuffer) {
			stbi_image_free(localBuffer);
		}

		return textureID;
	}

	unsigned int loadEmptyTexture(unsigned int width, unsigned int height, unsigned int scaleMode) {
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleMode);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
}