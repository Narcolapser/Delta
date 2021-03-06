#version 120

attribute vec3 coord3d;
attribute vec3 vNorm;
//attribute vec3 v_color;
uniform mat4 mvp;
uniform mat4 local;
varying vec4 color;
//varying vec3 line;

struct lightSource
{
  vec4 position;
  vec4 diffuse;
};
lightSource light0 = lightSource(
    vec4(-0.2, -1.0, -0.5, 0.0),
    vec4(1.0, 1.0, 1.0, 1.0)
);
 
struct material
{
  vec4 diffuse;
};
material mymaterial = material(vec4(1.0, 1.0, 1.0, 1.0));

void main(void) 
{
	vec3 normalDirection = normalize(vec3(local * vec4(vNorm,0.0)));
	vec3 lightDirection = normalize(vec3(light0.position));

	vec3 diffuseReflection = vec3(light0.diffuse) * vec3(mymaterial.diffuse) * max(0.0,dot(normalDirection,lightDirection));

	color = vec4(diffuseReflection, 1.0);

	gl_Position = mvp * local * vec4(coord3d, 1.0);	
}
