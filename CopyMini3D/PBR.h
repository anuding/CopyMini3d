#pragma once
#include "matrix.h"
#include <algorithm>
float DistributionGGX(Vector3 N, Vector3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(Vector3 N, Vector3 V, Vector3 L, float roughness);
Vector3 fresnelSchlick(float cosTheta, Vector3 F0);