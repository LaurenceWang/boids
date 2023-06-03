/*#version 330 core

layout(location = 0) in vec3 aVertexPosition; // Position du sommet
//layout(location = 1) in vec3 aVertexNormal; // Normale du sommet
//layout(location = 2) in vec2 aVertexTexCoords;

// Values that stay constant for the whole mesh.
uniform mat4 lightSpaceMatrix;

void main(){
    gl_Position =  lightSpaceMatrix * vec4(aVertexPosition,1.);
}*/

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}