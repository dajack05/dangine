#version 330 core

uniform sampler2D colorTexture;

in vec2 uvs;

out vec4 FragColor;

void main()
{
    FragColor = texture(colorTexture, uvs);
}