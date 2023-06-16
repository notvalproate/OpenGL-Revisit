#pragma once
#include <string_view>

namespace textureloader {
    unsigned int loadTexture(std::string_view a_Path, unsigned int a_ScaleMode, unsigned int a_WrapMode);
}