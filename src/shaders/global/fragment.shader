#version 410 core

in vec3 v_FinalColor;

out vec4 output;

void main() {
	output = vec4(v_FinalColor, 1.0);
}