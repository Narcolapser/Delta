#version 120 
#extension GL_EXT_geometry_shader4 : enable

void main(void)
{
	
	//increment variable
	int i;

	//Pass-thru!
	for(i=0; i< gl_VerticesIn; i++)
	{
		int x, y;
		gl_Position = gl_PositionIn[i];
		x = gl_Position.x;
		y = gl_Position.y;
		gl_Position.xy = vec(x+0.1,y+0.1);
		EmitVertex();
		gl_Position.xy = vec(x-0.1,y+0.1);
		EmitVertex();
		gl_Position.xy = vec(x+0.1,y-0.1);
		EmitVertex();
		gl_Position.xy = vec(x-0.1,y-0.1);
		EmitVertex();
	}
	EndPrimitive();	
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
GEOMETRY SHADER DOCUMENTATION

Geometry language built-in outputs:
	varying out vec4 gl_FrontColor;
	varying out vec4 gl_BackColor;
	varying out vec4 gl_FrontSecondaryColor;
	varying out vec4 gl_BackSecondaryColor;
	varying out vec4 gl_TexCoord[]; 	// at most gl_MaxTextureCoords
	varying out float gl_FogFragCoord; 

Geometry language input varying variables:
	varying in vec4 gl_FrontColorIn[gl_VerticesIn];
	varying in vec4 gl_BackColorIn[gl_VerticesIn];
	varying in vec4 gl_FrontSecondaryColorIn[gl_VerticesIn];
	varying in vec4 gl_BackSecondaryColorIn[gl_VerticesIn];
	varying in vec4 gl_TexCoordIn[gl_VerticesIn][]; 	// at most will be// gl_MaxTextureCoords
	varying in float gl_FogFragCoordIn[gl_VerticesIn];
	varying in vec4 gl_PositionIn[gl_VerticesIn];
	varying in float gl_PointSizeIn[gl_VerticesIn];
	varying in vec4 gl_ClipVertexIn[gl_VerticesIn];

Geometry Shader Function 
	
	This section contains functions that are geometry language specific.
	
	Syntax:
	 void EmitVertex(); 
	 void EndPrimitive(); 
	
	Description:
	
	 The function EmitVertex() specifies that a vertex is completed. A vertex is added to the current output primitive using the current values of the varying output variables and the current values of the special built-in output variables gl_PointSize, gl_ClipVertex, gl_Layer, gl_Position and gl_PrimitiveID. The values of any unwritten output variables are undefined. The values of all varying output variables and the special built-in output variables are undefined after a call to EmitVertex(). If a geometry shader, in one invocation, emits more vertices than the valueGEOMETRY_VERTICES_OUT_EXT, these emits may have no effect. 
 
	The function EndPrimitive() specifies that the current output primitive is completed and a new output primitive (of the same type) should be started. This function does not emit a vertex. The effect of EndPrimitive() is roughly equivalent to calling End followed by a new Begin, where the primitive mode is taken from the program object parameter GEOMETRY_OUTPUT_TYPE_EXT. If the output primitive type is POINTS, calling EndPrimitive() is optional.
*/
