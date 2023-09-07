#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
}





#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

float Lum(vec4 color) { // Get luminance of color
    return dot(color.rgb, vec3(0.299, 0.587, 0.114));
}

float Average(vec4 color) { // Get average of R G B of color
    return (color.r + color.g + color.b) / 3;
}

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    float luminance = Lum(texColor);
    float threshold = 0.5; // Adjust this threshold as needed (usually in the range [0, 1])

    vec4 outColor;
    if (luminance > threshold) {
        outColor = vec4(1.0); // White
    } else {
        outColor = vec4(0.0); // Black
    }
    outColor.a = texColor.a;
    gl_FragColor = mix(texColor, outColor, texColor.a);
}

