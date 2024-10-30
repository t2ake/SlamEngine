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
#include "Lib/Camera.glsl"
#include "Lib/Material.glsl"
#include "Lib/BRDF.glsl"
#include "Lib/Light.glsl"

float GetDistanceAttenuation(vec3 uLightDir, float range)
{
    float distance2 = dot(uLightDir, uLightDir);
    float attenuation = 1.0 / (max(distance2, 0.01 * 0.01));

    // n = 4
    float factor = distance2 / (range * range);
    float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
    float smoothDistanceAtt = smoothFactor * smoothFactor;

    return attenuation * smoothDistanceAtt;
}

float GetAngleAttenuation(vec3 lightDir, vec3 lightForward, float angleScale, float angleOffset)
{
    // On the CPU
    // float angleScale = 1.0f / max(0.001f, (cosInner - cosOuter));
    // float angleOffset =- cosOuter * angleScale;
    float cd = dot(lightDir, lightForward);
    float attenuation = clamp(cd * angleScale + angleOffset, 0.0, 1.0);

    return attenuation * attenuation;
}

vec3 GetDirectLightContribute(vec3 worldPos, vec3 cameraPos, Material material)
{
    vec3 direct = vec3(0.0);
    for(uint i = 0; i < ub_lightCount; ++i)
    {
        Light light = ub_lights[i];

        vec3 lightPos = light.position.xyz;
        vec3 lightForward = normalize(light.direction.xyz);

        vec3 viewDir = normalize(cameraPos - worldPos);
        vec3 ulightDir = lightPos - worldPos;
        vec3 lightDir = normalize(ulightDir);
        vec3 harfDir  = normalize(lightDir + viewDir);
        float NdotV = max(dot(material.normal, viewDir), 0.0);
        float NdotL = max(dot(material.normal, lightDir), 0.0);
        float NdotH = max(dot(material.normal, harfDir), 0.0);
        float VdotH = max(dot(viewDir, harfDir), 0.0);

        float Dis = DistributionGGX(material.roughness, NdotH);
        float Vis = VisibilitySchlick(material.roughness, NdotV, NdotL);
        vec3 Fre = FresnelSchlick(material.F0, VdotH);
        vec3 KD = mix(vec3(1.0) - Fre, vec3(0.0), material.metallic);
        vec3 BRDF = vec3(Dis) * vec3(Vis) * Fre + KD * material.albedo * vec3(SL_PI_INV);

        if(light.type == LIGHT_TYPE_DIRECTIONAL)
        {
            // L_out = f * E * cosTheta
            direct += BRDF * light.intensity * max(dot(material.normal, -lightForward), 0.0);
        }
        else if(light.type == LIGHT_TYPE_POINT)
        {
            // I = Phi / 4Pi
            // L_out = f * L_in * cosTheta

            float I = light.intensity * 0.25 * SL_PI_INV;
            float attenuation = GetDistanceAttenuation(ulightDir, light.range); 
            float L = I * attenuation;

            direct += BRDF * L * NdotL;
        }
        else if(light.type == LIGHT_TYPE_SPOT)
        {
            // I = Phi / Pi
            // L_out = f * L_in * cosTheta

            float I = light.intensity * SL_PI_INV;
            float attenuation = GetDistanceAttenuation(ulightDir, light.range);
            attenuation *= GetAngleAttenuation(lightDir, lightForward, light.scale, light.offset);
            float L = I * attenuation;

            direct += BRDF * L * NdotL;
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
    vec3 finalColor = directColor + environmentColor;
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    o_color = vec4(finalColor, 1.0);
}
