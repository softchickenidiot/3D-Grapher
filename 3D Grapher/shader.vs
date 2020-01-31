#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in int Shader;

uniform mat4 gWVP;
uniform mat4 xRotate;
uniform mat4 yRotate;
uniform mat4 zRotate;

out vec4 color;

void main()
{
   color = Color;
   if (Shader == 0 ) {
        if (abs(Position.x) > .9) {
             gl_Position = xRotate * vec4(Position, 1.0);
             gl_Position = gWVP * gl_Position;
        }   
        if (abs(Position.y) > .9) {
             gl_Position = yRotate * vec4(Position, 1.0);
             gl_Position = gWVP * gl_Position;
        }   
        if (abs(Position.z) > .9) {
             gl_Position = zRotate * vec4(Position, 1.0);
             gl_Position = gWVP * gl_Position;
        }
    }
    if (Shader != 0) {
        gl_Position = gWVP * vec4(Position, 1.0);
    }
 
};