#version 330 core

in VSOUT
{
	vec4 position;
}fsIn;

out vec4 fragColor;

void main()
{
	
	fragColor = (fsIn.position + 0.5) / 2.0f;
	//fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

} 