/*#version 330

in vec3 vPosition_vs; 
in vec3 vNormal_vs; 
in vec2 vTexCoords;
in vec4 fragPosLightSpace;

out vec4 fFragColor;


uniform sampler2D uTexture;


uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightPosArp_vs;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity; 


uniform sampler2D shadowMap;

vec3 blinnPhongDir()
{


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



float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vNormal_vs);
    vec3 lightDir = normalize(uLightPos_vs - vPosition_vs);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}



void main()
{

    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 lightPos = blinnPhongPos();
    vec3 lightPos2 = blinnPhongPosArp();
    vec3 lightDir = blinnPhongDir();
    vec4 texture = texture(uTexture, vTexCoords);

    vec3 lightning = lightDir  + lightPos * (1.0 - shadow)+ lightPos2;


    fFragColor = ( vec4( lightning, 1)  )* texture; 
   // fFragColor = vec4(shadow, shadow, shadow, 1);
    //fFragColor = texture; 
}*/



#version 330

in vec3 vPosition_vs; 
in vec3 vNormal_vs; 
in vec2 vTexCoords;
in vec4 fragPosLightSpace;

out vec4 fFragColor;


uniform sampler2D uTexture;


uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightPosArp_vs;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity; 


uniform sampler2D shadowMap;

vec3 blinnPhongDir()
{


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


vec3 blinnPhongPos(vec3 lightPos, vec3 lightIntensity)
{
 
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(lightPos - vPosition_vs);
    vec3 halfVector = (w0 + wi)/2.0;
    float nDotL = max(0.0, dot(wi, vNormal_vs));
    float nDotH = max(0.0, dot(halfVector, vNormal_vs));
    float specular = pow(nDotH, 1.0);
    vec3 diffuse = vec3(0.5) * nDotL;
    vec3 specularColor = uKs * specular;
    float d = distance(vPosition_vs,lightPos);

    return (lightIntensity / (d * d)) * (diffuse + specularColor);

}



float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vNormal_vs);
    vec3 lightDir = normalize(uLightPos_vs - vPosition_vs);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}



void main()
{

    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 lightPos = blinnPhongPos(uLightPos_vs, uLightIntensity);

    vec3 lightPos2 =blinnPhongPos(uLightPosArp_vs, uLightIntensity);
    //vec3 lightPos2 = blinnPhongPosArp();
    vec3 lightDir = blinnPhongDir();
    vec4 texture = texture(uTexture, vTexCoords);

    vec3 lightning = lightDir  + lightPos * (1.0 - shadow)+ lightPos2;


    fFragColor = ( vec4( lightning, 1)  )* texture; 
   // fFragColor = vec4(shadow, shadow, shadow, 1);
    //fFragColor = texture; 
}