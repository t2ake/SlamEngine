float DistributionGGX(float roughness, float NdotH)
{
    // a = roughness^2
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = SL_PI * denom * denom;
    
    return a2 / denom;
}

float GeometrySchlick(float roughness, float NdotV, float NdotL)
{
    float r = (roughness + 1.0) * 0.5;
    float a = r * r;
    float denomV = NdotV * (2.0 - a) + a;
    float denomL = NdotL * (2.0 - a) + a;
    
    return (4.0 * NdotV * NdotL) / (denomV * denomL);
}

float VisibilitySchlick(float roughness, float NdotV, float NdotL)
{
    // BRDF = (D * G * F) / (4 * NdotV * NdotL) = D * V * F
    // V = G / (4 * NdotV * NdotL)
    //   = 1 / ((NdotV * (2 - a) + a) * (NdotL * (2 - a) + a))
    
    // roughness = (roughness + 1) / 2, by Disney
    // a = roughness^2
    float r = (roughness + 1.0) * 0.5;
    float a = r * r;
    float denomV = NdotV * (2.0 - a) + a;
    float denomL = NdotL * (2.0 - a) + a;
    
    return 1.0 / (denomV * denomL);
}

// TODO: Height correlated geometry.

vec3 FresnelSchlick(vec3 F0, float VdotH)
{
    return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

vec3 FresnelSchlickSphericalGaussian(vec3 F0, float VdotH)
{
    return F0 + (1.0 - F0) * exp2(-5.55473 * VdotH - 6.998316 * VdotH);
}
