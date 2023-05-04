#version 330

in vec3 vPosition_vs; 
in vec3 vNormal_vs; 
//in vec2 vTexCoords; 

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity; 

vec3 blinnPhong(){

    /*vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    float d = distance(vPosition_vs,uLightPos_vs);
    vec3 halfVector = (wi + w0 ) / 2.f;

    
    vec3 specularColor = uKd* dot(wi, vNormal_vs);

    vec3 diffuse = uKs *  pow(dot(halfVector, vNormal_vs), uShininess);
   

    vec3 couleur = (uLightIntensity / (d * d)) * (diffuse + specularColor);

 
    return couleur;*/

    /*float d = distance(vPosition_vs, uLightPos_vs);
    vec3 Li = (uLightIntensity / (d * d));
    vec3 N = vNormal_vs;
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    //return Li*(uKd*max(dot(wi, N), 0.) + uKs*pow(max(dot(halfVector, N), 0.), uShininess));

    return  uLightPos_vs / d;*/


    float d = distance(vPosition_vs, uLightPos_vs);
    vec3 Li = (uLightIntensity / (d * d));
    vec3 N = vNormal_vs;
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    return  Li*(uKd*max(dot(wi, N), 0.) + uKs*pow(max(dot(halfVector, N), 0.), uShininess));
    
    //return  Li;
    

}

void main() {
    fFragColor = vec4(blinnPhong(),1);
    //fFragColor = vec4(normalize(uKs),1);
}
