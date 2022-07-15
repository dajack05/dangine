#version 330 core

uniform mat4 MVP;
uniform vec3 tint;
uniform vec4 UVPosSize;

in vec2 vUvs;
in vec3 vPos;

out vec3 color;
out vec2 uvs;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    color = tint;
    uvs = vUvs * UVPosSize.zw + UVPosSize.xy;
}