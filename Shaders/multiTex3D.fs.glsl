#version 330
in vec2 vTexCoords;

out vec4 fFragColor;


uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main()
{
    vec4 color1 = texture(uEarthTexture, vTexCoords); 
    vec4 color2 = texture(uCloudTexture, vTexCoords); 
    fFragColor = color1 + color2; 