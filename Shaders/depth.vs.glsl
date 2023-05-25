#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uModel;
uniform vec3 LightInvDirection_worldspace;
uniform mat4 DepthBiasMVP;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  uMVPMatrix * vec4(vertexPosition_modelspace,1);
	
	ShadowCoord = DepthBiasMVP * vec4(vertexPosition_modelspace,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (uModel * vec4(vertexPosition_modelspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	EyeDirection_cameraspace = vec3(0,0,0) - ( uMVMatrix * uModel * vec4(vertexPosition_modelspace,1)).xyz;

	// Vector that goes from the vertex to the light, in camera space
	LightDirection_cameraspace = (uMVMatrix*vec4(LightInvDirection_worldspace,0)).xyz;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( uMVMatrix * uModel * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
