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
  float ambientStrength = 0.15;
  vec3 ambient = ambientStrength * lightColor;

  vec3 diffuseSum = vec3(0.0);
  vec3 specularSum = vec3(0.0);

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(viewPos - FragPos);

  for (int i = 0; i < lightCount; ++i) {
    vec3 lightDir = normalize(lightPositions[i] - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    diffuseSum += lightColor * (diff * material.diffuse);

    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);
    specularSum += specularStrength * (spec * lightColor);
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