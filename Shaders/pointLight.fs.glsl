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

    /*float d = distance(vPosition_vs, uLightPos_vs);
    vec3 Li = (uLightIntensity / (d * d));
    vec3 N = vNormal_vs;
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    return  Li*(uKd*max(dot(wi, N), 0.) + uKs*pow(max(dot(halfVector, N), 0.), uShininess));*/

    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = vec3(0.5) * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,uLightPos_vs);

    return (uLightIntensity / (d * d)) * (diffuse + specularColor);
    
}

void main() {
    fFragColor = vec4(blinnPhong(),1);
    //fFragColor = vec4(normalize(uKs),1);
}
