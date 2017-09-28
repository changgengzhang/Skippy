#version 330 core
// in
layout (location=0) in vec4 position;
layout (location=1) in vec3 color;
// uniform values
uniform mat4 projMat;


// out
out VSOUT
{
	vec4 position;
	vec3 color;
}vsOut;


void main()
{
	gl_Position = projMat * position;
	//gl_Position = projMat * viewMat * position;
	vsOut.position = gl_Position;
	vsOut.color = color;
}