#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_fragCoord;
uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_fragCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_fragCoord;
uniform sampler2D u_Texture;
uniform int u_Frame;
uniform vec2 u_Resolution;

float Lum(vec4 c) { return dot(c.rgb, vec3(0.299, 0.587, 0.114)); }
float Ave(vec4 c) { return (c.r + c.g + c.b) / 3; }

void main() {
    vec4 texColor = texture(u_Texture, v_fragCoord);
    color = texColor;
}
