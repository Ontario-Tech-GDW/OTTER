#version 410

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outNormal;

uniform mat4 u_SkyboxMatrix;
uniform mat3 u_EnvironmentRotation;

void main() {
    vec4 pos = u_SkyboxMatrix * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;

    // Normals
    outNormal = u_EnvironmentRotation * inPosition;
}

