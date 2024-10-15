#version 450 core

// input
layout(location = 0) in vec2 v_uv0;

// output
layout(location = 0) out vec4 o_color;

// sampler
layout(binding = 0) uniform sampler2D s_texture;

void main()
{
	o_color = texture(s_texture, v_uv0);
}
