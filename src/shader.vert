#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertColor;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec2 verttexCoord;

out vec3 fragPos;
out vec3 fragColor;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    fragPos     = vec3(model * vec4(vertPos, 1.0));
    fragColor   = vertColor;
    Normal      = vertNormal;
    texCoord    = verttexCoord;

    gl_Position = proj * view * vec4(fragPos, 1.0);
}
