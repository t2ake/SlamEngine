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

vec3 GetDirectLightContribute(vec3 worldPos, vec3 cameraPos, Material material)
{
    vec3 direct = vec3(0.0);
    for(uint i = 0; i < ub_lightCount; ++i)
    {
        Light light = ub_lights[i];
        if(light.type == LIGHT_TYPE_DIRECTIONAL)
        {
            direct += vec3(0.0);
        }
        else if(light.type == LIGHT_TYPE_POINT)
        {
            vec3 lightPos = light.position.xyz;
            vec3 viewDir = normalize(cameraPos - worldPos);
            vec3 lightDir = normalize(lightPos - worldPos);
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

            direct += BRDF;
        }
        else if(light.type == LIGHT_TYPE_SPOT)
        {
            direct += vec3(0.0);
        }
    }

    return direct;
}

void main()
{
    Material material = GetMaterial(v_uv0);
    vec3 directColor = GetDirectLightContribute(v_worldPos, GetCameraPos(), material);

    vec3 environmentColor = vec3(0.01);

    // Post processing
    vec3 finalColor = directColor + environmentColor;
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    o_color = vec4(finalColor, 1.0);
}
