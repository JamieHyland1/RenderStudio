#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;

void main()
{
    vec3 color = vec3(
        1,
        1,
        1
    );

    FragColor = vec4(lightColor, 1.0);
}