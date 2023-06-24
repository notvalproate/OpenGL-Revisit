#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	v_TexCoord = aTexCoord;
	v_TexIndex = aTexIndex;
	v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	v_FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
}