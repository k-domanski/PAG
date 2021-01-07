#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in mat4 aInstanceModel;

out vec3 fNormal;
out vec3 fPos;
out vec2 fTextCoord;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPos = vec3(aInstanceModel * vec4(position, 1.0));
	gl_Position = projection * view * vec4(fPos, 1.0);
	//gl_Position = projection * view * aInstanceModel * vec4(position, 1.0);
	fNormal = mat3(transpose(inverse(aInstanceModel))) * aNormal;

	fTextCoord = aTextCoord;
};

#shader fragment
#version 330 core
out vec4 FragColor;

struct Material
{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

struct PointLight
{
	vec3 position;

	float constant;
	float _linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool isActive;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	bool isActive;
	float intensity;
};

in vec3 fNormal;
in vec3 fPos;
in vec2 fTextCoord;

#define NUM_DIR_LIGHT 1
#define NUM_SPOT_LIGHT 3
#define NUM_POINT_LIGHT 2

uniform Material material;
uniform DirLight dirLight[NUM_DIR_LIGHT];
uniform PointLight pointLight[NUM_POINT_LIGHT];
//uniform SpotLight spotLight[NUM_SPOT_LIGHT];
//uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D Texture;
//uniform vec3 camPos;

const float PI = 3.14159265359;
vec3 calculateDirLight(vec3 albedo, vec3 N);
vec3 calculatePointLight(vec3 albedo, vec3 N);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
	vec3 result = vec3(0.0);
	vec3 albedo = pow(texture(Texture, fTextCoord).rgb, vec3(2.2));
	vec3 N = normalize(fNormal);
	for (int i = 0; i < NUM_POINT_LIGHT; ++i)
	{
		if (pointLight[i].isActive)
		{
			result += calculatePointLight(albedo, N);
		}
		
	}
	for (int i = 0; i < NUM_DIR_LIGHT; i++)
	{
		if(dirLight[i].isActive)
			result += calculateDirLight(albedo, N);
	}
	
	FragColor = vec4(result, 1.0);
};

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 calculatePointLight(vec3 albedo, vec3 N)
{
	vec3 V = normalize(viewPos - fPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, material.metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for (int j = 0; j < NUM_POINT_LIGHT; j++)
	{
		// calculate per-light radiance
		vec3 L = normalize(pointLight[j].position - fPos);
		vec3 H = normalize(V + L);
		float distance = length(pointLight[j].position - fPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = pointLight[j].diffuse * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, material.roughness);
		float G = GeometrySmith(N, V, L, material.roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - material.metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	// ambient lighting (note that the next IBL tutorial will replace 
	// this ambient lighting with environment lighting).
	vec3 ambient = vec3(0.03) * albedo * material.ao;
	
	vec3 color = ambient + Lo;
	
	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));
	return color;
}

vec3 calculateDirLight(vec3 albedo, vec3 N)
{
	vec3 V = normalize(viewPos - fPos);
	

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, material.metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for (int j = 0; j < NUM_DIR_LIGHT; j++)
	{
		//vec3 V = normalize(dirLight[j].direction);
		// calculate per-light radiance
		vec3 L = normalize(-dirLight[j].direction);
		vec3 H = normalize(V + L);
		//float distance = length(pointLight[j].position - fPos);
		//float attenuation = 1.0 / (distance * distance);
		vec3 radiance = dirLight[j].diffuse;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, material.roughness);
		float G = GeometrySmith(N, V, L, material.roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - material.metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	// ambient lighting (note that the next IBL tutorial will replace 
	// this ambient lighting with environment lighting).
	vec3 ambient = vec3(0.03) * albedo * material.ao;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));
	return color;
}
