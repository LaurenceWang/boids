#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
//in vec3 vPosition_vs; 

uniform samplerCube skybox;

/*uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity; 

vec3 blinnPhong(){

    float d = distance(vPosition_vs, uLightPos_vs);
    vec3 Li = (uLightIntensity / (d * d));
    vec3 N =  vec3(1,1,1);
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    return  Li*(uKd*max(dot(wi, N), 0.) + uKs*pow(max(dot(halfVector, N), 0.), uShininess));
    
}*/


void main()
{    
    //FragColor = vec4(1,0,1,0);
    //vec3 lightPos = blinnPhong();
    FragColor = texture(skybox, TexCoords);
 
}