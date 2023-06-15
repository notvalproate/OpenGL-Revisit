#version 410 core

out vec4 output;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;

void main() {
	float amb = 0.1f;
	vec3 lightcolor = vec3(1.0f, 1.0f, 1.0f);

	vec4 tex = texture(u_Texture, v_TexCoord);

	vec3 normm = normalize(v_Normal);
	vec3 t_LightDir = normalize(u_LightPos - v_FragPos);
	float diff = max(dot(normm, t_LightDir), 0.0f);

	float distance = length(u_LightPos - v_FragPos);	
	float att = 1.0 / (1.0 + (0.09 * distance) + (0.032 * distance * distance));

	vec3 ambient = amb * lightcolor;
	vec3 diffuse = att * diff * lightcolor;

	output = vec4(ambient + diffuse, 1.0f) * tex;
}