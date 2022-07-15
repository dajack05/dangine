#version 330 core

uniform sampler2D colorTexture;

in vec3 color;
in vec2 uvs;

out vec4 FragColor;

void main()
{
    // FragColor = vec4(uvs, 0.0, 1.0);
    FragColor = vec4(color, 1.0) * texture(colorTexture, uvs);
}