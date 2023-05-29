#version 330

in vec3 vPosition_vs; 
in vec3 vNormal_vs; 
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uMoonTexture;
uniform sampler2D uFishTexture;


uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightPosArp_vs;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity; 

vec3 blinnPhongDir()
{
    /*vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, (1.0));
    vec3 diffuse = uKs* nDotL;
    vec3 specularColor = uKd * specular;

    return uLightIntensity * (diffuse + specularColor);*/

    /*vec3 wi= normalize(uLightDir_vs);
    vec3 Li= vec3(uLightIntensity);
    vec3 w0 = (normalize(-vPosition_vs));
    vec3 N = normalize(vNormal_vs);
    vec3 halfVector = (w0+wi)/2.f;
    return Li*uKs*(dot(wi, N));   */

    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, (1.0));
    vec3 diffuse = vec3(1) * nDotL;
    vec3 specularColor = vec3(1) * specular;

    return uLightIntensity * (diffuse + specularColor);
}
vec3 blinnPhongPosArp()
{
    /*vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = uKd * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,uLightDir_vs);

    return (uLightIntensity / (d * d)) * (diffuse + specularColor);*/

     vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPosArp_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = vec3(0.5) * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,uLightPosArp_vs);

    return (vec3(1) / (d * d)) * (diffuse + specularColor);
}


vec3 blinnPhongPos()
{
    /*vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = uKd * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,uLightPos_vs);

    return (uLightIntensity / (d * d)) * (diffuse + specularColor);*/
 
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = vec3(0.5) * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,uLightPos_vs);

    return (vec3(0.6) / (d * d)) * (diffuse + specularColor);

}

/*vec3 blinnPhongPosArp()
{
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightCharacter_vs - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = vec3(0.5) * nDotL;
    vec3 specularColor = vec3(1.0 , 0.7 , 0.0 ) * specular;
    float d = distance(vPosition_vs,uLightCharacter_vs);

    return (0.005  / (d * d)) * (diffuse + specularColor);
}*/

void main()
{
    /*vec4 color1 = texture(uFishTexture, vTexCoords); 
    vec4 color2 = texture(uMoonTexture, vTexCoords); 
   // vec4 color3 = texture(uFishTexture, vTexCoords);
    fFragColor = color1;*/  
    vec3 lightPos = blinnPhongPos();
    vec3 lightPos2 = blinnPhongPosArp();
    vec3 lightDir = blinnPhongDir();
    vec4 texture = texture(uFishTexture, vTexCoords);
    fFragColor = ( vec4(  lightDir + lightPos + lightPos2, 1)  )* texture; 
    //fFragColor = texture; 
}