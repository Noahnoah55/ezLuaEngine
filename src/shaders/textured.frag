#version 300 es
precision highp float;

uniform sampler2D inTexture;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
    FragColor = texture(inTexture, vec2(texCoord.x, texCoord.y));
}