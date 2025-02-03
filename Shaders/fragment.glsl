#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec3 color = vec3(
        mod(FragPos.x, 1.0),  // Map X to R
        mod(FragPos.y, 1.0),  // Map Y to G
        mod(FragPos.z, 1.0)   // Map Z to B
    );

    FragColor = vec4(color, 1.0);
}