#version 120

attribute vec3 coord3d;
//attribute vec3 v_color;
uniform mat4 mvp;
uniform mat4 local;
//varying vec3 f_color;
//varying vec3 line;

void main(void) 
{
	gl_Position = mvp * local * vec4(coord3d, 1.0);

//	f_color = v_color;
}
