layout(std140, binding = SL_UBO_BINDING_POINT_CAMERA) uniform UBCamera
{
    vec4 ub_cameraPos;
    mat4 ub_viewProjection;
};

vec3 GetCameraPos()
{
    return ub_cameraPos.xyz;
}

mat4 GetViewProjectionMat()
{
    return ub_viewProjection;
}
