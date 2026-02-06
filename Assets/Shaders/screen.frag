#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTex;
uniform int uForceSolid;
uniform vec3 uSolidColor;
uniform int uDebugUV;

void main()
{
    // Force solid color mode (highest priority)
    if (uForceSolid == 1)
    {
        FragColor = vec4(uSolidColor, 1.0);
        return;
    }
    
    // UV debug mode
    if (uDebugUV == 1)
    {
        FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
        return;
    }
    
    // Normal texture sampling
    FragColor = texture(uTex, TexCoord);
}



