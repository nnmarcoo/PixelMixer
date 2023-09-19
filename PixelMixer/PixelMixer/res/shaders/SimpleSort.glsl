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
//float Ave(vec4 c) { return (c.r + c.g + c.b) / 3; }

void main() {
    vec4 texColor = texture(u_Texture, v_fragCoord);
    
    color = texColor;
}

/*
 uniform sampler2D PackedData;    // contents of the texcoord data  
 #define OwnPos gl_TexCoord[0].xy  
 #define SearchDir gl_TexCoord[0].z  
 #define CompOp gl_TexCoord[0].w  
 #define Distance gl_TexCoord[1].x    
 #define Stride gl_TexCoord[1].y  
 #define Height gl_TexCoord[1].z  
 #define HalfStrideMHalf gl_TexCoord[1].w    
 void main(void)  {    // get self    
     vec4 self = texture2D(PackedData, OwnPos);                            // restore sign of search direction and assemble vector to partner      
     vec2 adr = vec2( (SearchDir < 0.0) ? -Distance : Distance, 0.0);      // get the partner    
     vec4 partner = texture2D(PackedData, OwnPos + adr);                   // switch ascending/descending sort for every other row      // by modifying comparison flag    
     float compare = CompOp * -(mod(floor(gl_TexCoord[0].y * Height), Stride) - HalfStrideMHalf);      // x and y are the keys of the two items    // --> multiply with comparison flag      
     vec4 keys = compare * vec4( self.x, self.y, partner.x, partner.y);      // compare the keys and store accordingly    // z and w are the indices    // --> just copy them accordingly    
     vec4 result;    
     result.xz = (keys.x < keys.z) ? self.xz : partner.xz;    
     result.yw = (keys.y < keys.w) ? self.yw : partner.yw;      // do pass 0      
     compare *= adr.x;    gl_FragColor = (result.x * compare < result.y * compare) ? result : result.yxwz;
     
 } 
 */

