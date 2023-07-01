#version 410 core

out vec4 color;

centroid in vec2 v_TexCoord;
centroid in vec3 v_Normal;
centroid in vec3 v_FragPos;

//STRUCTURES
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float dissolve;
	float shininess;

	sampler2D diffuseMap;
	bool hasDiffuse;
	sampler2D specularMap;
	bool hasSpecular;
	sampler2D normalMap;
	bool hasNormal;
};

struct DirectionalLight {
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Brightness;
};

struct PointLight {
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Kc, Kl, Kq;

	float Brightness;
};

struct SpotLight {
	vec3 Position;
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float InnerCutOff;
	float OuterCutOff;

	float Kc, Kl, Kq;

	float Brightness;
};

//UNIFORMS
uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLight[50];
uniform SpotLight u_SpotLight;

uniform vec3 u_ViewPos;

//FUNCTION DECLARATIONS
vec4 getAmbience(const vec3 lightColor, const vec4 diffuseMap);
vec4 getDiffusion(const vec3 lightColor, const vec3 lightDir, const vec4 diffuseMap);
vec4 getSpecular(const vec3 lightColor, const vec3 lightDir, const vec4 specularMap);

vec4 getDirectionalLight(const vec4 diffuseMap, const vec4 specularMap);
vec4 getPointLight(const PointLight pointLight, const vec4 diffuseMap, const vec4 specularMap);
vec4 getSpotLight(const SpotLight spotLight, const vec4 diffuseMap, const vec4 specularMap);

void main() {
	vec4 diffuseMap = vec4(u_Material.diffuse, 1.0f);
	vec4 specularMap = vec4(u_Material.specular, 1.0f);

	if (u_Material.hasDiffuse) {
		diffuseMap *= texture(u_Material.diffuseMap, v_TexCoord);
	}
	if (u_Material.hasSpecular) {
		specularMap *= texture(u_Material.specularMap, v_TexCoord);
	}

	color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	color += getDirectionalLight(diffuseMap, specularMap);

	for (int i = 0; i < 50; i += 1) {
		if (u_PointLight[i].Brightness != 0) {
			color += getPointLight(u_PointLight[i], diffuseMap, specularMap);
		}
	}

	color += getSpotLight(u_SpotLight, diffuseMap, specularMap);

	color = vec4(color.rgb, u_Material.dissolve);
}

//GET AMBIENCE DIFFUSION AND SPECULAR VECTORS
vec4 getAmbience(const vec3 lightColor, const vec4 diffuseMap) {
	return vec4(lightColor, 1.0) * diffuseMap;
}

vec4 getDiffusion(const vec3 lightColor, const vec3 lightDir, const vec4 diffuseMap) {
	float diffStrength = max(dot(v_Normal, lightDir), 0.0);

	return vec4(lightColor, 1.0) * (diffStrength * diffuseMap);
}

vec4 getSpecular(const vec3 lightColor, const vec3 lightDir, const vec4 specularMap) {
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, v_Normal);

	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

	return vec4(lightColor, 1.0) * (specStrength * specularMap);
}

//GET DIRECTIONAL LIGHT VECTOR
vec4 getDirectionalLight(const vec4 diffuseMap, const vec4 specularMap) {
	vec3 lightDir = normalize(-u_DirectionalLight.Direction);

	vec4 ambient = getAmbience(u_DirectionalLight.Ambient, diffuseMap);

	vec4 diffuse = getDiffusion(u_DirectionalLight.Diffuse, lightDir, diffuseMap);

	vec4 specular = getSpecular(u_DirectionalLight.Specular, lightDir, specularMap);

	return u_DirectionalLight.Brightness * (ambient + diffuse + specular);
}

//GET POINT LIGHT VECTOR
vec4 getPointLight(const PointLight pointLight, const vec4 diffuseMap, const vec4 specularMap) {
	vec3 lightDir = normalize(pointLight.Position - v_FragPos);

	float distance = length(pointLight.Position - v_FragPos);
	float attenuation = 1.0 / (pointLight.Kc + (pointLight.Kl * distance) + (pointLight.Kq * distance * distance));

	vec4 ambient = getAmbience(pointLight.Ambient, diffuseMap);

	vec4 diffuse = getDiffusion(pointLight.Diffuse, lightDir, diffuseMap);

	vec4 specular = getSpecular(pointLight.Specular, lightDir, specularMap);

	return pointLight.Brightness * attenuation * (ambient + diffuse + specular);
}

//GET POINT LIGHT VECTOR
vec4 getSpotLight(const SpotLight spotLight, const vec4 diffuseMap, const vec4 specularMap) {
	vec3 lightDir = normalize(spotLight.Position - v_FragPos);

	float distance = length(spotLight.Position - v_FragPos);
	float attenuation = 1.0 / (spotLight.Kc + (spotLight.Kl * distance) + (spotLight.Kq * distance * distance));

	float theta = dot(lightDir, normalize(-spotLight.Direction));
	float epsilon = spotLight.InnerCutOff - spotLight.OuterCutOff;
	float intensity = clamp((theta - spotLight.OuterCutOff) / epsilon, 0.0, 1.0);

	vec4 ambient = getAmbience(spotLight.Ambient, diffuseMap);

	vec4 diffuse = intensity * getDiffusion(spotLight.Diffuse, lightDir, diffuseMap);

	vec4 specular = intensity * getSpecular(spotLight.Specular, lightDir, specularMap);

	return spotLight.Brightness * attenuation * (ambient + diffuse + specular);
}