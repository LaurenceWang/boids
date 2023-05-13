#version 330
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uMoonTexture;
uniform sampler2D uFishTexture;

void main()
{
    vec4 color1 = texture(uFishTexture, vTexCoords); 
    vec4 color2 = texture(uMoonTexture, vTexCoords); 
   // vec4 color3 = texture(uFishTexture, vTexCoords);
    fFragColor = color1;
}