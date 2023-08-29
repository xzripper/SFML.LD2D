// SFML Glow Shader (Unfinished).

uniform sampler2D texture;
uniform float offset;

void main() {
    vec4 baseColor = texture2D(texture, gl_TexCoord[0].xy);
    vec4 blurredColor = vec4(0.0);

    for (int i = -4; i <= 4; ++i) {
        blurredColor += texture2D(texture, gl_TexCoord[0].xy + vec2(offset * float(i), 0.0));
    }

    blurredColor /= 9.0;

    vec4 glow = baseColor + blurredColor * 2.0;

    gl_FragColor = glow;
}
