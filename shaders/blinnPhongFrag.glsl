#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shining;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoffAngle;
    float outerCutoffAngle;
};

struct HasTextureMaps
{
    bool hasDiffuseMap;
    bool hasSpecularTexture;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 viewPosition;

const int maxLights = 128;
uniform PointLight pointLights[maxLights];
uniform SpotLight spotLights[maxLights];
uniform DirectionalLight directionalLight;

uniform uint pointLightsNum = 0u;
uniform uint spotLightsNum = 0u;

uniform Material material;
uniform HasTextureMaps textureMaps;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 calcAmbient(vec3 lightAmbient, vec3 materialAmbient);
vec3 calcDiffuse(vec3 lightDir, vec3 lightDiffuse);
vec3 calcSpecular(vec3 viewDir, vec3 lightDir, vec3 lightSpecular);
float calcAttenuation(vec3 lightPosition, float lightConstant, float lightLinear, float lightQuadratic);
vec3 calcDirLight(DirectionalLight light, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(viewPosition - fragPos);

    vec3 lightData = calcDirLight(directionalLight, viewDir);
    for(uint i = 0u; i < pointLightsNum; i++)
    {
        lightData += calcPointLight(pointLights[i], viewDir);
    }
    for(uint i = 0u; i < spotLightsNum; i++)
    {
        lightData += calcSpotLight(spotLights[i], viewDir);
    }

    FragColor = vec4(lightData, 1.0);
}

vec3 calcAmbient(vec3 lightAmbient, vec3 materialAmbient)
{
    return lightAmbient * materialAmbient;
}

vec3 calcDiffuse(vec3 lightDir, vec3 lightDiffuse)
{
    float diffuseValue = max(dot(normalize(normal), lightDir), 0.0);
    if(textureMaps.hasDiffuseMap)
    {
        return vec3(texture(texture_diffuse1, texCoord)) * diffuseValue * lightDiffuse;
    }
    else
    {
        return material.diffuse * diffuseValue * lightDiffuse;

    }
}

vec3 calcSpecular(vec3 viewDir, vec3 lightDir, vec3 lightSpecular)
{
    vec3 halfwayVector = normalize(viewDir + lightDir);
    float specularValue = pow(max(dot(halfwayVector, normalize(normal)), 0.0), material.shining);
    if(textureMaps.hasSpecularTexture)
    {
        return vec3(texture(texture_specular1, texCoord)) * lightSpecular * specularValue;
    }
    else
    {
        return material.specular * lightSpecular * specularValue;
    }
}

float calcAttenuation(vec3 lightPosition, float lightConstant, float lightLinear, float lightQuadratic)
{
    float lightDist = length(lightPosition - fragPos);
    return (1.0 / (lightConstant + lightLinear * lightDist + lightQuadratic * pow(lightDist, 2.0)));
}

vec3 calcDirLight(DirectionalLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = calcAmbient(light.ambient, material.ambient);
    vec3 diffuse = calcDiffuse(lightDir, light.diffuse);
    vec3 specular = calcSpecular(viewDir, lightDir, light.specular);

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = calcAmbient(light.ambient, material.ambient);
    vec3 diffuse = calcDiffuse(lightDir, light.diffuse);
    vec3 specular = calcSpecular(viewDir, lightDir, light.specular);

    float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = calcAmbient(light.ambient, material.ambient);
    vec3 diffuse = calcDiffuse(lightDir, light.diffuse);
    vec3 specular = calcSpecular(viewDir, lightDir, light.specular);

    float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoffAngle - light.outerCutoffAngle;
    float intensity = clamp((theta - light.outerCutoffAngle) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
