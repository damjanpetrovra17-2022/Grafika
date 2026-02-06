#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// Material properties
uniform vec3 uBaseColor;

// Light properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform bool lightEnabled;

// View position (camera)
uniform vec3 viewPos;

void main()
{
    if (!lightEnabled)
    {
        // If light is disabled, use very dark ambient
        vec3 ambient = 0.1 * uBaseColor;
        FragColor = vec4(ambient, 1.0);
        return;
    }
    
    // Ambient - reduced to prevent over-brightness
    float ambientStrength = 0.12;
    vec3 ambient = ambientStrength * lightColor * uBaseColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * uBaseColor;
    
    // Specular (Blinn-Phong) - reduced strength
    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Attenuation (distance-based) - adjusted for less falloff
    float distance = length(lightPos - FragPos);
    float attenuation = lightIntensity / (1.0 + 0.14 * distance + 0.05 * distance * distance);
    
    // Combine and clamp to prevent excessive brightness
    vec3 result = (ambient + diffuse + specular) * attenuation;
    result = clamp(result, 0.0, 1.0);  // Prevent over-saturation
    
    FragColor = vec4(result, 1.0);
}
