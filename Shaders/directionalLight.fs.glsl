#version 330


in vec3 vPosition_vs; 
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uTexture;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs; 
uniform vec3 uLightIntensity; 



vec3 blinnPhong() {
    //vec3 Li = uLightIntensity;
    
    vec3 Li = uLightIntensity;
    vec3 N = normalize(vNormal_vs);
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfVector = (w0 + wi)/2.f;

    return Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
    //return uLightDir_vs;
}

void main() {

    fFragColor = vec4(blinnPhong(), 1) * texture(uTexture, vTexCoords) ;

    //fFragColor = vec4(1, 1,1, 1);
    //fFragColor = texture(uTexture, vTexCoords);
}