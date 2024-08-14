#version 450 core

// input
layout(location = 0) in vec3 a_position;

// uniform buffer
layout(std140, binding = 0) uniform Camera
{
	mat4 u_viewProjection;
	vec4 u_cameraPos;
};

// uniform
layout(location = 0) uniform mat4 u_model;

void main()
{
	gl_Position = u_viewProjection * u_model * vec4(a_position, 1.0);
}
