#pragma once
#include <string_view>

namespace textureloader {
    unsigned int loadTexture(std::string_view path, unsigned int scaleMode, unsigned int wrapMode);
    unsigned int loadEmptyTexture(unsigned int width, unsigned int height, unsigned int scaleMode);
}