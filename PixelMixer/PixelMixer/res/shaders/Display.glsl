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

void main() {
    color = texture(u_Texture, v_fragCoord);
}
