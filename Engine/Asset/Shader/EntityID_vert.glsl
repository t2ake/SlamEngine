#version 450 core

#include <Camera.glsl>

// input
layout(location = 0) in vec3 a_position;

// uniform
layout(location = 0) uniform mat4 u_model;

void main()
{
	gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
