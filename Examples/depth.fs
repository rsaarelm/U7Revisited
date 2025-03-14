#version 330

// Synthesize a combined depth and normal buffer to the output. Write
// surface's normal-x, normal-y to red and green channels and surface depth to
// the blue channel.

// Inputs from vertex shader
in vec3 screenPosition;
in vec3 screenNormal;

// Output color (to render target)
out vec4 fragColor;

void main() {
    // Raylib defaults, these should be made into shader uniforms.
    float near = 0.01;
    float far = 1000.0;

    // Normalize the normal vector
    vec3 normalizedNormal = normalize(screenNormal);

    // Get x, y components of normal mapped to [0.0, 1.0].
    vec2 screenNormal = normalizedNormal.xy * 0.5 + 0.5;

    // XXX: Does this make sense as the [0.0, 1.0] z-range?
    float depth = (screenPosition.z - near) / (far - near);

    fragColor = vec4(depth, screenNormal, 1.0);
}
