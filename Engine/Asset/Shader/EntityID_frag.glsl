#version 450 core

// output
layout(location = 0) out int o_entityID;

// uniform
layout(location = 1) uniform int u_entityID;

void main()
{
	o_entityID = u_entityID;
}
