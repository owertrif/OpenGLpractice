#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;

out vec2 TexCord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
   TexCord = vec2(aTexCord.x,aTexCord.y);
}