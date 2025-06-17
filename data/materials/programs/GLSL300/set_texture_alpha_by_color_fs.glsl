#version 330 core

in vec2 ComputedTexCoord;

out vec4 ColorOut;

//layout(binding = 0) 
uniform sampler2D MainTexture;
uniform sampler2D ReflTexture;
uniform sampler2D BumpTexture;

//layout(binding = 1)
uniform vec3 AlphaColor;
uniform int AlphaColorPresent;

void main() {

    //ColorOut = vec4(ComputedTexCoord, 1.0, 1.0);
    
    
    vec4 computedColor = texture(MainTexture, ComputedTexCoord);
    if (AlphaColorPresent == 1)
    {
        if (distance(computedColor.xyz, AlphaColor) < 0.05f)
        {
            discard;
        }
    }
    
    if (computedColor.w < 0.05f)
    {
        discard;
    }
    
    ColorOut = computedColor;
}