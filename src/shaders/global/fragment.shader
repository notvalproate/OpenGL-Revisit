#version 410 core

out vec4 output;

in vec3 v_FinalColor;
in vec2 v_FinalTexCoord;

uniform sampler2D v_Texture;

void main() {
	vec4 tex = texture(v_Texture, v_FinalTexCoord);

	output = tex;
}