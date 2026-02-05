#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3 uPosSize;   // x, y, width
uniform float uHeight;   // height

void main()
{
    vec2 scaled = aPos * vec2(uPosSize.z, uHeight);
    vec2 world = scaled + vec2(uPosSize.x, uPosSize.y);
    gl_Position = vec4(world, 0.0, 1.0);

    TexCoord = aTexCoord;
}
