#pragma once
#include <string_view>

namespace TextureLoader {
    unsigned int LoadTexture(std::string_view path, unsigned int scaleMode, unsigned int wrapMode);
}