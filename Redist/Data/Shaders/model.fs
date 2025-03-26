#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec3 lightDir;
uniform vec3 ambient;
uniform vec3 viewPos;

out vec4 finalColor;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDirNorm = normalize(-lightDir);

    float diff = max(dot(normal, lightDirNorm), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDirNorm, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);

    vec3 result = (diffuse + specular + ambient) * texture(texture0, fragTexCoord).rgb;
    finalColor = vec4(result, 1.0);
}
