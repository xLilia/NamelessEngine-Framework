#version 450 core   

layout (location=0) in vec3 Position;
layout (location=4) uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
}