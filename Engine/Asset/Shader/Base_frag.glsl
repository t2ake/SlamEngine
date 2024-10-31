#version 450 core

// input
layout(location = 0) in vec3 v_worldPos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv0;

// output
layout(location = 0) out vec4 o_color;

#include "Shared/PBRMaterial.h"
#include "Shared/Light.h"

#include "Lib/Core.glsl"
#include "Lib/Material.glsl"
#include "Lib/BRDF.glsl"
#include "Lib/Light.glsl"
#include "Lib/Camera.glsl"

float GetDistanceAttenuation(float distance2, float range)
{
    // n = 4

    float attenuation = 1.0 / (max(distance2, 0.01 * 0.01));
    float factor = distance2 / (range * range);
    float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
    float smoothDistanceAtt = smoothFactor * smoothFactor;

    return attenuation * smoothDistanceAtt;
}

float GetAngleAttenuation(vec3 lightDir, vec3 lightForward, float angleScale, float angleOffset)
{
    // On the CPU
    // float angleScale = 1.0 / max(0.001, (cosInner - cosOuter));
    // float angleOffset = -cosOuter * angleScale;

    float cd = dot(lightDir, -lightForward);
    float attenuation = clamp(cd * angleScale + angleOffset, 0.0, 1.0);

    return attenuation * attenuation;
}

vec3 GetBRDFDirect(vec3 lightDir, vec3 viewDir, Material material)
{
    vec3 harfDir  = normalize(lightDir + viewDir);
    float NdotV = max(dot(material.normal, viewDir), 0.0);
    float NdotL = max(dot(material.normal, lightDir), 0.0);
    float NdotH = max(dot(material.normal, harfDir), 0.0);
    float VdotH = max(dot(viewDir, harfDir), 0.0);

    float Dis = DistributionGGX(material.roughness, NdotH);
    float Vis = VisibilitySchlick(material.roughness, NdotV, NdotL);
    vec3 Fre = FresnelSchlick(material.F0, VdotH);
    vec3 KD = mix(vec3(1.0) - Fre, vec3(0.0), material.metallic);
    
    return vec3(Dis) * vec3(Vis) * Fre + KD * material.albedo * vec3(SL_PI_INV);
}

vec3 GetDirectLightContribute(vec3 worldPos, vec3 cameraPos, Material material)
{
    vec3 direct = vec3(0.0);
    for(uint i = 0; i < ub_lightCount; ++i)
    {
        Light light = ub_lights[i];

        vec3 lightColor = light.color.xyz;
        vec3 lightPos = light.position.xyz;
        vec3 lightForward = normalize(light.direction.xyz);

        vec3 ulightDir = lightPos - worldPos;
        vec3 lightDir = normalize(ulightDir);
        vec3 viewDir = normalize(cameraPos - worldPos);
        float NdotL = max(dot(material.normal, lightDir), 0.0);

        float distance2 = dot(ulightDir, ulightDir);
        float distanceAttenuation = GetDistanceAttenuation(distance2, light.range);
        float angleAttenuation = GetAngleAttenuation(lightDir, lightForward, light.scale, light.offset);

        if(light.type == LIGHT_TYPE_DIRECTIONAL)
        {
            // L_out = f * E * cosTheta

            lightDir = -lightForward;
            NdotL = max(dot(material.normal, lightDir), 0.0);
            vec3 BRDF = GetBRDFDirect(lightDir, viewDir, material);

            direct += BRDF * lightColor * light.intensity * NdotL;
        }
        else if(light.type == LIGHT_TYPE_POINT)
        {
            // I = Phi / 4Pi
            // L_out = f * L_in * cosTheta

            vec3 BRDF = GetBRDFDirect(lightDir, viewDir, material);
            float I = light.intensity * 0.25 * SL_PI_INV;
            float L = I * distanceAttenuation;

            direct += BRDF * lightColor * L * NdotL;
        }
        else if(light.type == LIGHT_TYPE_SPOT)
        {
            // I = Phi / Pi
            // L_out = f * L_in * cosTheta

            vec3 BRDF = GetBRDFDirect(lightDir, viewDir, material);
            float I = light.intensity * SL_PI_INV;
            float L = I * distanceAttenuation * angleAttenuation;

            direct += BRDF * lightColor * L * NdotL;
        }
    }

    return direct;
}

void main()
{
    vec3 cameraPos = GetCameraPos();
    Material material = GetMaterial(v_uv0);
    material.normal = v_normal; // TMP
    vec3 directColor = GetDirectLightContribute(v_worldPos, cameraPos, material);

    vec3 environmentColor = vec3(0.01);

    // Post processing
    vec3 finalColor = directColor + environmentColor + material.emissive;
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    o_color = vec4(finalColor, 1.0);
}
