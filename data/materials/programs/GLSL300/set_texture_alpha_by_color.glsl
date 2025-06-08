#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 AlphaColor;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.xyz == AlphaColor)
        discard;
    FragColor = texColor;
}
