// SFML Pixelate Shader (https://github.com/SFML/SFML/blob/master/examples/shader/resources/pixelate.frag).

uniform sampler2D texture;

uniform float pixelThreshold;

void main() {
    float factor = 1.0 / (pixelThreshold + 0.001);

    vec2 position = floor(gl_TexCoord[0].xy * factor + 0.5) / factor;

    gl_FragColor = texture2D(texture, position) * gl_Color;
}
