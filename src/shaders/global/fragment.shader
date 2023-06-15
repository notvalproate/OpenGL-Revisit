#version 410 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;

struct PointLight {
	vec3 Position;
	vec3 Color;

	float AmbientStrength;
	float SpecularStrength;

	float Kc, Kl, Kq;

	float Brightness;
};

uniform PointLight u_PointLight;

uniform vec3 u_ViewPos;

vec4 GetAmbience(const vec3 p_LightColor, const float p_AmbientStrength);
vec4 GetDiffusion(const vec3 p_LightColor, const vec3 p_LightDir, const float p_Att);
vec4 GetSpecular(const vec3 p_LightColor, const vec3 p_LightDir, const float p_Att, const float p_SpecStrength);

vec4 GetPointLight(const PointLight p_PointLight);

void main() {
	vec4 tex = texture(u_Texture, v_TexCoord);

	vec4 final = GetPointLight(u_PointLight);

	color = final * tex;
}

//GET AMB DIFF AND SPEC
vec4 GetAmbience(const vec3 p_LightColor, const float p_AmbientStrength) {
	return vec4(p_AmbientStrength * p_LightColor, 1.0);
}

vec4 GetDiffusion(const vec3 p_LightColor, const vec3 p_LightDir, const float p_Att) {
	float t_DiffStrength = max(dot(v_Normal, p_LightDir), 0.0);

	return vec4(p_Att * t_DiffStrength * p_LightColor, 1.0);
}

vec4 GetSpecular(const vec3 p_LightColor, const vec3 p_LightDir, const float p_Att, const float p_SpecStrength) {
	vec3 t_ViewDir = normalize(u_ViewPos - v_FragPos);
	vec3 t_ReflectDir = reflect(-p_LightDir, v_Normal);

	float t_Spec = pow(max(dot(t_ViewDir, t_ReflectDir), 0.0), 32);

	return vec4(p_Att * p_SpecStrength * t_Spec * p_LightColor, 1.0);
}

//GET POINT, SPOT AND DIR LIGHT
vec4 GetPointLight(const PointLight p_PointLight) {
	vec3 t_LightDir = normalize(p_PointLight.Position - v_FragPos);

	float t_Distance = length(p_PointLight.Position - v_FragPos);
	float t_Att = 1.0 / (p_PointLight.Kc + (p_PointLight.Kl * t_Distance) + (p_PointLight.Kq * t_Distance * t_Distance));

	vec4 ambient = GetAmbience(p_PointLight.Color, p_PointLight.AmbientStrength);

	vec4 diffuse = GetDiffusion(p_PointLight.Color, t_LightDir, t_Att);

	vec4 specular = GetSpecular(p_PointLight.Color, t_LightDir, t_Att, p_PointLight.SpecularStrength);

	return p_PointLight.Brightness * (ambient + diffuse + specular);
}
