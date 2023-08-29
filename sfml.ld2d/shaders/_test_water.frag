// Water Shader. Unfinished. https://www.shadertoy.com/view/dlsfzB

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord.xy / iResolution.xy;

    float frequency = 11.0;

    float amplitude = 0.009;

    float speed = 1.5;

    float waveX = amplitude * sin(uv.y * frequency + iTime * speed);
    float waveY = amplitude * cos(uv.x * frequency + iTime * speed);

    vec3 textureColor = texture(iChannel0, vec2(uv.x + waveX, uv.y + waveY)).rgb;

    fragColor = vec4(mix(textureColor, textureColor * vec3(0.0, 0.5, 0.91), 0.5), 1.0);
}
