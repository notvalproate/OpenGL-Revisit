#version 410 core

out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Material {
	sampler2D diffusion;
	sampler2D specular;
	float shininess;
};

uniform Material u_Materials[32];

struct pointlight {
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Kc, Kl, Kq;

	float Brightness;
};

uniform pointlight u_PointLight[50];

uniform vec3 u_ViewPos;

vec4 getAmbience(const vec3 lightColor, const vec4 diffuseMap);
vec4 getDiffusion(const vec3 lightColor, const vec3 lightDir, const vec4 diffuseMap);
vec4 getSpecular(const vec3 lightColor, const vec3 lightDir, const vec4 specularMap, const float shininess);

vec4 getPointLight(const pointlight pointLight, const vec4 diffuseMap, const vec4 specularMap, const float shininess);

void main() {
	int index = int(v_TexIndex);
	vec4 tex = texture(u_Materials[index].diffusion, v_TexCoord);
	vec4 spec = texture(u_Materials[index].specular, v_TexCoord);
	float shininess = u_Materials[index].shininess;

	vec4 final = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 50; i += 1) {
		if (u_PointLight[i].Brightness != 0) {
			final += getPointLight(u_PointLight[i], tex, spec, shininess);
		}
	}

	color = vec4(final.r, final.g, final.b, tex.a);
}

//GET AMB DIFF AND SPEC
vec4 getAmbience(const vec3 lightColor, const vec4 diffuseMap) {
	return vec4(lightColor, 1.0) * diffuseMap;
}

vec4 getDiffusion(const vec3 lightColor, const vec3 lightDir, const vec4 diffuseMap) {
	float diffStrength = max(dot(v_Normal, lightDir), 0.0);

	return vec4(lightColor, 1.0) * (diffStrength * diffuseMap);
}

vec4 getSpecular(const vec3 lightColor, const vec3 lightDir, const vec4 specularMap, const float shininess) {
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, v_Normal);

	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	return vec4(lightColor, 1.0) * (specStrength * specularMap);
}

//GET POINT LIGHT
vec4 getPointLight(const pointlight pointLight, const vec4 diffuseMap, const vec4 specularMap, const float shininess) {
	vec3 lightDir = normalize(pointLight.Position - v_FragPos);

	float distance = length(pointLight.Position - v_FragPos);
	float attenuation = 1.0 / (pointLight.Kc + (pointLight.Kl * distance) + (pointLight.Kq * distance * distance));

	vec4 ambient = getAmbience(pointLight.Ambient, diffuseMap);

	vec4 diffuse = getDiffusion(pointLight.Diffuse, lightDir, diffuseMap);

	vec4 specular = getSpecular(pointLight.Specular, lightDir, specularMap, shininess);

	return pointLight.Brightness * attenuation *(ambient + diffuse + specular);
}