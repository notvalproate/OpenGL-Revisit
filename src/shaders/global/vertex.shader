#version 410 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Color;

out vec3 v_FinalColor;

void main() {
	v_FinalColor = v_Color;
	gl_Position = vec4(v_Position, 1.0);
}