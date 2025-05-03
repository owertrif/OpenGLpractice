#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCord;

out vec3 vertexColor;
out vec2 TexCord;

uniform mat4 transform;

void main()
{
   gl_Position = transform*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   vertexColor = aColor;
   TexCord = vec2(aTexCord.x,aTexCord.y);
}