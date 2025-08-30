#version 330 core

out vec4 FragColor;

in vec3 vertColor;
in vec2 texCoord; // Add this line

uniform sampler2D numberAtlas;
uniform int useTexture;

void main()
{
    if (useTexture == 1) {
        float atlasValue = texture(numberAtlas, texCoord).r;
        if (atlasValue > 0.5) {
            FragColor = vec4(vertColor * 0.3, 1.0);
        } else {
            FragColor = vec4(vertColor, 1.0);
        }
    } else {
        FragColor = vec4(vertColor, 1.0);
    }
}