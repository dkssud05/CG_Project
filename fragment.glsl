#version 330 core

out vec4 FragColor;
in vec3 outColor;

uniform vec3 uColor; // CPU에서 색상 값을 받음

void main()
{
    FragColor = vec4(outColor, 1.0);
}