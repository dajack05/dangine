#version 330 core
in vec3 vPos;
in vec2 vUvs;

out vec2 uvs;
void main()
{
   uvs = vUvs;
   gl_Position = vec4(vPos.x * 2, -vPos.y * 2, 1.0, 1.0);
}