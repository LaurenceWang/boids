#version 330 core
layout (location = 0) in vec3 aVertexPosition;

out vec3 TexCoords;
//out vec3 vPosition_vs;

uniform mat4 uProjection;
uniform mat4 uMVMatrix;

void main()
{
    //vec4 vertexPosition = vec4(aVertexPosition, 1.);
    //vPosition_vs = vec3(uMVMatrix * vertexPosition);
    TexCoords = aVertexPosition;
    vec4 pos = uProjection * uMVMatrix * vec4(aVertexPosition, 1.0);
    gl_Position = pos.xyww;
}  