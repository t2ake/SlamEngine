#version 450 core

#include <Camera.glsl>

// input
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

// output
layout(location = 0) out vec2 v_uv;

// uniform
layout(location = 0) uniform mat4 u_model;

void main()
{
	v_uv = a_uv;
	gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
