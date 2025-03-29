#version 330

// Depth render vertex shader.

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;

out vec3 screenPosition;
out vec3 screenNormal;

void main() {
    screenPosition = vec3(mvp * vec4(vertexPosition, 1.0));
    screenNormal = normalize(mat3(mvp * matModel) * vertexNormal);
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
