#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

void main()
{
    //fFragColor = vec4(normalize(vNormal_vs), 1.);
    fFragColor = vec4(1,0,1, 1.);
}