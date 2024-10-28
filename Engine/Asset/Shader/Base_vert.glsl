#version 450 core

#include <Camera.glsl>

// input
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_uv0;

// output
layout(location = 0) out vec3 v_worldPos;
layout(location = 1) out vec3 v_normal;
layout(location = 2) out vec2 v_uv0;

// uniform
layout(location = 0) uniform mat4 u_model;
layout(location = 1) uniform mat4 u_modelInvTrans;

void main()
{
    v_worldPos = (u_model * vec4(a_position, 1.0)).xyz;
    v_normal = (u_modelInvTrans * vec4(a_normal, 1.0)).xyz;;
    v_uv0 = a_uv0;
    
    gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
