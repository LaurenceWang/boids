#version 330 core

// Attributs de sommet
layout(location = 0) in vec3 aVertexPosition; // Position du sommet
layout(location = 1) in vec3 aVertexNormal; // Normale du sommet
layout(location = 2) in vec2 aVertexTexCoords; // Coordonnées de texture du sommet

// Matrices de transformation reçues en uniform
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

uniform mat4 LightSpaceMatrix;

// Sorties du shader
out vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
out vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
out vec2 vTexCoords; // Coordonnées de texture du sommet
out vec4 fragPosLightSpace;

void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    fragPosLightSpace = LightSpaceMatrix *vertexPosition;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}