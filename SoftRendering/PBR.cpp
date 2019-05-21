#include "PBR.h"
const float PI = 3.14159265359;
float DistributionGGX(Vector3 N, Vector3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = std::max(Dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / std::max(denom, 0.001f); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(Vector3 N, Vector3 V, Vector3 L, float roughness)
{
	float NdotV = std::max(Dot(N, V), 0.0f);
	float NdotL = std::max(Dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
Vector3 fresnelSchlick(float cosTheta, Vector3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}