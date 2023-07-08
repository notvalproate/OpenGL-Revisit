#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 v_TexCoords;

void main() {
    v_TexCoords = aTexCoords;
    gl_Position = vec4(aPosition.xy, 0.0, 1.0);
}