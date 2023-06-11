#version 410 core

out vec4 output;

in vec3 v_FinalColor;
in vec2 v_FinalTexCoord;

uniform sampler2D v_Texture1;
uniform sampler2D v_Texture2;

void main() {
	vec4 tex1 = texture(v_Texture1, v_FinalTexCoord);
	vec4 tex2 = texture(v_Texture2, v_FinalTexCoord);

	output = (tex2.a == 1.0 ? tex2 : mix(tex1, tex2, tex2.a));
}