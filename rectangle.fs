#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D tex;
uniform float useTexture;  // 0 = color, 1 = texture

void main()
{
    if (useTexture > 0.5)
        FragColor = texture(tex, TexCoord);
    else
        FragColor = uColor;
}
