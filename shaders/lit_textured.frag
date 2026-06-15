#version 330 core
out vec4 fragmentColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;
in vec3 FragWorldPos;

uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int lightCount;
uniform vec3 lightPositions[8];
uniform vec3 lightColors[8];

uniform vec3 uAmbientColor;
uniform float uAmbientStrength;

// fog
uniform int uFogEnabled;
uniform vec3 uFogColor;
uniform float uFogDensity;
uniform float uFogStart;
uniform float uFogEnd;

uniform sampler2D uTexture;

struct Material {
    // vec3 ambient;
    vec3 diffuse;
    // vec3 specular;
    float shininess;
}; 

uniform Material material;
uniform samplerCube skybox;

void main() {
  vec3 ambient = uAmbientColor * uAmbientStrength;

  vec3 diffuseSum = vec3(0.0);
  vec3 specularSum = vec3(0.0);

  vec3 norm = normalize(Normal);

  for (int i = 0; i < lightCount; ++i) {
    vec3 lightDir = normalize(lightPositions[i] - FragPos);
    float distance = length(lightPositions[i] - FragPos);

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // light color changes throught distance (yellow close, white far)
    vec3 warmColor = lightColors[i];
    vec3 coolColor = vec3(1.0, 1.0, 1.0);
    float colorMix = clamp(distance / 8.0, 0.0, 1.0); // 8 unit mix
    vec3 finalLightColor = mix(warmColor, coolColor, colorMix);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = finalLightColor * (diff * material.diffuse) * attenuation;
    diffuseSum += diffuse;

    // specular math
    float specularStrength = 0.15;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularStrength * (spec * finalLightColor) * attenuation;
    specularSum += specular;
  }

  float distanceToCamera = length(viewPos - FragWorldPos);
  float fogAmount = 1.0 - exp(-distanceToCamera * uFogDensity);
  fogAmount = clamp(fogAmount, 0.0, 1.0);

  vec3 result = (ambient + diffuseSum + specularSum) * objectColor;
  vec4 baseColor = texture(uTexture, TexCoords) * vec4(result, 1.0);

  if (uFogEnabled == 1) {
    float distanceToCamera = length(viewPos - FragWorldPos);
    float fogAmount = 1.0 - exp(-distanceToCamera * uFogDensity);
    fogAmount = clamp(fogAmount, 0.0, 1.0);

    baseColor.rgb = mix(baseColor.rgb, uFogColor, fogAmount);
  }

  fragmentColor = baseColor;
} 