#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float visibility;

void main()
{
   FragColor = mix(texture(texture1, TexCord),texture(texture2,vec2(TexCord.x,TexCord.y)),visibility);
}