#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aMaterialIndex;
 
centroid out vec2 v_TexCoord;
centroid out vec3 v_Normal;
centroid out vec3 v_FragPos;
centroid out int v_MaterialIndex;

uniform mat3 u_NormalMatrix;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	v_MaterialIndex = int(aMaterialIndex);
	v_TexCoord = aTexCoord;
	v_Normal = normalize(u_NormalMatrix * aNormal);
	v_FragPos = vec3(u_Model * vec4(aPosition, 1.0));
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
}