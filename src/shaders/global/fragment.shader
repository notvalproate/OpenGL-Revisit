#version 410 core

out vec4 output;

in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
	vec4 tex = texture(u_Texture, v_TexCoord);
	output = tex;
}