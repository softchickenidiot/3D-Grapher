#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;

uniform mat4 gWVP;
uniform mat4 xRotate;

out vec4 color;

void main()
{
   color = Color;
   if (abs(Position.x) > .9) {
        gl_Position = xRotate * vec4(Position, 1.0);
        gl_Position = gWVP * gl_Position;
        color = vec4(1.0, 0.0, 0.0, 1.0);
   }
   else {
        gl_Position = gWVP * vec4(Position, 1.0);
   }
};