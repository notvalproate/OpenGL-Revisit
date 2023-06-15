#version 410 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture; 
uniform sampler2D u_SpecMap;

struct PointLight {
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Kc, Kl, Kq;

	float Brightness;
};

uniform PointLight u_PointLight;

uniform vec3 u_ViewPos;

vec4 GetAmbience(const vec3 p_LightColor, const vec4 p_DiffuseMap);
vec4 GetDiffusion(const vec3 p_LightColor, const vec3 p_LightDir, const vec4 p_DiffuseMap);
vec4 GetSpecular(const vec3 p_LightColor, const vec3 p_LightDir, const vec4 p_SpecularMap);

vec4 GetPointLight(const PointLight p_PointLight, const vec4 p_DiffuseMap, const vec4 p_SpecularMap);

void main() {
	vec4 tex = texture(u_Texture, v_TexCoord);
	vec4 spec = texture(u_SpecMap, v_TexCoord);

	vec4 final = GetPointLight(u_PointLight, tex, spec);

	color = final;
}

//GET AMB DIFF AND SPEC
vec4 GetAmbience(const vec3 p_LightColor, const vec4 p_DiffuseMap) {
	return vec4(p_LightColor, 1.0) * p_DiffuseMap;
}

vec4 GetDiffusion(const vec3 p_LightColor, const vec3 p_LightDir, const vec4 p_DiffuseMap) {
	float t_DiffStrength = max(dot(v_Normal, p_LightDir), 0.0);

	return vec4(p_LightColor, 1.0) * (t_DiffStrength * p_DiffuseMap);
}

vec4 GetSpecular(const vec3 p_LightColor, const vec3 p_LightDir, const vec4 p_SpecularMap) {
	vec3 t_ViewDir = normalize(u_ViewPos - v_FragPos);
	vec3 t_ReflectDir = reflect(-p_LightDir, v_Normal);

	float t_SpecStrength = pow(max(dot(t_ViewDir, t_ReflectDir), 0.0), 32);

	return vec4(p_LightColor, 1.0) * (t_SpecStrength * p_SpecularMap);
}

//GET POINT LIGHT
vec4 GetPointLight(const PointLight p_PointLight, const vec4 p_DiffuseMap, const vec4 p_SpecularMap) {
	vec3 t_LightDir = normalize(p_PointLight.Position - v_FragPos);

	float t_Distance = length(p_PointLight.Position - v_FragPos);
	float t_Att = 1.0 / (p_PointLight.Kc + (p_PointLight.Kl * t_Distance) + (p_PointLight.Kq * t_Distance * t_Distance));

	vec4 ambient = GetAmbience(p_PointLight.Ambient, p_DiffuseMap);

	vec4 diffuse = t_Att * GetDiffusion(p_PointLight.Diffuse, t_LightDir, p_DiffuseMap);

	vec4 specular = t_Att * GetSpecular(p_PointLight.Specular, t_LightDir, p_SpecularMap);

	return p_PointLight.Brightness * (ambient + diffuse + specular);
}
