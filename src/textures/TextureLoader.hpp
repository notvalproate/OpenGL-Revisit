#pragma once
#include <string_view>

namespace textureloader {
    unsigned int loadTexture(std::string_view path, unsigned int scaleMode, unsigned int wrapMode);
}