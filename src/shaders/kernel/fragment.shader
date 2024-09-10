#version 410 core

out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D frame;

float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9];
    offsets[0] = vec2(-offset, offset);
    offsets[1] = vec2(0.0, offset);
    offsets[2] = vec2(offset, offset);
    offsets[3] = vec2(-offset, 0.0);
    offsets[4] = vec2(0.0, 0.0);
    offsets[5] = vec2(offset, 0.0);
    offsets[6] = vec2(-offset, -offset);
    offsets[7] = vec2(0.0, -offset);
    offsets[8] = vec2(offset, -offset);

    float kernel[9];
    kernel[0] = 0.11;
    kernel[1] = 0.11;
    kernel[2] = 0.11;
    kernel[3] = 0.11;
    kernel[4] = 0.11;
    kernel[5] = 0.11;
    kernel[6] = 0.11;
    kernel[7] = 0.11;
    kernel[8] = 0.11;

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(frame, v_TexCoords + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        col += kernel[i] * sampleTex[i];
    }

    float a = (col.r + col.g + col.b) / 3.0;

    color = vec4(a, 0.0, 0.0, 1.0);
}
