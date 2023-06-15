#version 410 core

out vec4 color;

uniform vec3 u_Color;

void main() {
	color = vec4(u_Color, 1.0);
}