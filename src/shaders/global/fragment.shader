#version 410 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;

struct PointLight {
	vec3 Position;
	vec3 Color;

	vec3 Diffusion;
	vec3 Ambient;

	float Kc, Kl, Kq;

	float Brightness;
};

uniform PointLight u_PointLight;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main() {
	vec3 lightcolor = vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.2f;
	float specularStrength = 0.5f;
	
	vec4 tex = texture(u_Texture, v_TexCoord);

	vec3 normal = normalize(v_Normal);
	vec3 lightdir = normalize(u_LightPos - v_FragPos);

	float diffusionStrength = max(dot(normal, lightdir), 0.0);
	float distance = length(u_LightPos - v_FragPos);	
	float att = 1.0 / (1.0 + (0.09 * distance) + (0.032 * distance * distance));

	vec3 viewdir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectdir = reflect(-lightdir, normal);

	float spec = pow(max(dot(viewdir, reflectdir), 0.0), 32);

	vec4 ambient = vec4(ambientStrength * lightcolor, 1.0);
	vec4 diffuse = vec4(att * diffusionStrength * lightcolor, 1.0);
	vec4 specular = vec4(specularStrength * spec * lightcolor, 1.0);

	color = (ambient + diffuse + specular) * tex;
}