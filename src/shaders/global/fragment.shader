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

vec4 getAmbience(const vec3 a_LightColor, const vec4 a_DiffuseMap);
vec4 getDiffusion(const vec3 a_LightColor, const vec3 a_LightDir, const vec4 a_DiffuseMap);
vec4 getSpecular(const vec3 a_LightColor, const vec3 a_LightDir, const vec4 a_SpecularMap, const float a_Shininess);

vec4 getPointLight(const pointlight a_PointLight, const vec4 a_DiffuseMap, const vec4 a_SpecularMap, const float a_Shininess);

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
vec4 getAmbience(const vec3 a_LightColor, const vec4 a_DiffuseMap) {
	return vec4(a_LightColor, 1.0) * a_DiffuseMap;
}

vec4 getDiffusion(const vec3 a_LightColor, const vec3 a_LightDir, const vec4 a_DiffuseMap) {
	float diffStrength = max(dot(v_Normal, a_LightDir), 0.0);

	return vec4(a_LightColor, 1.0) * (diffStrength * a_DiffuseMap);
}

vec4 getSpecular(const vec3 a_LightColor, const vec3 a_LightDir, const vec4 a_SpecularMap, const float a_Shininess) {
	vec3 ViewDir = normalize(u_ViewPos - v_FragPos);
	vec3 ReflectDir = reflect(-a_LightDir, v_Normal);

	float SpecStrength = pow(max(dot(ViewDir, ReflectDir), 0.0), a_Shininess);

	return vec4(a_LightColor, 1.0) * (SpecStrength * a_SpecularMap);
}

//GET POINT LIGHT
vec4 getPointLight(const pointlight a_PointLight, const vec4 a_DiffuseMap, const vec4 a_SpecularMap, const float a_Shininess) {
	vec3 lightDir = normalize(a_PointLight.Position - v_FragPos);

	float distance = length(a_PointLight.Position - v_FragPos);
	float attenuation = 1.0 / (a_PointLight.Kc + (a_PointLight.Kl * distance) + (a_PointLight.Kq * distance * distance));

	vec4 ambient = getAmbience(a_PointLight.Ambient, a_DiffuseMap);

	vec4 diffuse = getDiffusion(a_PointLight.Diffuse, lightDir, a_DiffuseMap);

	vec4 specular = getSpecular(a_PointLight.Specular, lightDir, a_SpecularMap, a_Shininess);

	return a_PointLight.Brightness * attenuation *(ambient + diffuse + specular);
}