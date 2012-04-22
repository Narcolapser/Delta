#version 120


varying vec4 color;
//varying vec3 line;

//uniform float local;

void main(void) 
{
//	gl_FragColor = vec4(0.5f,0.5f,1.0f,1.0f);
	gl_FragColor = color;
}
