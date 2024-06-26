#version 450 core

// input
layout(location = 0) in vec2 v_uv;

// output
layout(location = 0) out vec4 o_color;

// sampler
layout(binding = 0) uniform sampler2D u_texture;

void main()
{
	o_color = texture(u_texture, v_uv);
}
