#version 410 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in vec3 v_Color;

out vec3 v_FinalColor;
out vec2 v_FinalTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


void main() {
	v_FinalColor = v_Color;
	v_FinalTexCoord = v_TexCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(v_Position, 1.0);
}