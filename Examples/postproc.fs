#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D depthTexture;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here
uniform vec2 resolution = vec2(640, 360);

float depth(vec2 coord) {
    return texture(depthTexture, coord).x;
}

vec2 normal(vec2 coord) {
    return texture(depthTexture, coord).yz;
}

void main()
{
    // See if there are large depth differences near the current pixel.
    float d = depth(fragTexCoord);
    vec2 n = normal(fragTexCoord);

    // Compute the gradient of the depth field.
    float dx = d - depth(fragTexCoord + vec2(1, 0) / resolution);
    float dy = d - depth(fragTexCoord + vec2(0, 1) / resolution);

    // See if there is a big normal difference
    float nx = n.x - normal(fragTexCoord + vec2(1, 0) / resolution).x;
    float ny = n.y - normal(fragTexCoord + vec2(0, 1) / resolution).y;

    if (nx * nx + ny * ny > 0.2) {
        finalColor = vec4(0, 0, 0, 1);
    } else if (dx * dx + dy * dy > 0.01) {
        finalColor = vec4(0, 0, 0, 1);
    } else {
        finalColor = texture(texture0, fragTexCoord);
    }

}
