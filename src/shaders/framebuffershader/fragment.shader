#version 410 core

out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D tex;

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
    kernel[0] = 2;
    kernel[1] = 2;
    kernel[2] = 2;
    kernel[3] = 2;
    kernel[4] = -15;
    kernel[5] = 2;
    kernel[6] = 2;
    kernel[7] = 2;
    kernel[8] = 2;

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(tex, v_TexCoords + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        col += kernel[i] * sampleTex[i];
    }

    /*vec4 texcolor = texture(tex, v_TexCoords);
    float average = 0.2126 * texcolor.r + 0.7152 * texcolor.g + 0.0722 * texcolor.b;*/

    color = vec4(col, 1.0);
}
