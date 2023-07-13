#version 410 core

out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D frame;

void main() {
    vec4 texcolor = texture(frame, v_TexCoords);
    float average = 0.2126 * texcolor.r + 0.7152 * texcolor.g + 0.0722 * texcolor.b;

    color = vec4(vec3(average), 1.0);
}