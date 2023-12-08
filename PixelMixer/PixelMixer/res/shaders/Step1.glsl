#shader vertex
#version 330 core

out vec2 v_fragCoord;

void main() {
    vec2 vertices[6] = vec2[](
        vec2(-1.0, -1.0),
        vec2(1.0, -1.0),
        vec2(1.0, 1.0),
        vec2(-1.0, -1.0),
        vec2(1.0, 1.0),
        vec2(-1.0, 1.0)
    );
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
    v_fragCoord = (vertices[gl_VertexID] + 1.0) * 0.5;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_fragCoord;
uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, v_fragCoord);
    //color = vec4(1,.5,.3,1);
}
