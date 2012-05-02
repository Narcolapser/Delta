#include "Lib.h"

#ifndef LIB_CPP
#define LIB_CPP

char* file_read(const char* filename)
{
	FILE* in = fopen(filename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) 
	{
		if (nb_read_total + BUFSIZ > res_size) 
		{
			if (res_size > 10*1024*1024) break;
			res_size = res_size * 2;
			res = (char*)realloc(res, res_size);
		}
		char* p_res = res + nb_read_total;
		nb_read_total += fread(p_res, 1, BUFSIZ, in);
	}

	fclose(in);
	res = (char*)realloc(res, nb_read_total + 1);
	res[nb_read_total] = '\0';
	return res;
}
delta_t stringToEnum(string val)
{
	if(val.compare("Object")==0) return OBJECT;
	if(val.compare("Buffer")==0) return BUFFER;
	if(val.compare("Camera")==0) return CAMERA;
	if(val.compare("GeoObject")==0) return GEOOBJECT;
	if(val.compare("Mesh")==0) return MESH;
	if(val.compare("Model")==0) return MODEL;
	if(val.compare("Scene")==0) return SCENE;
	if(val.compare("Program")==0) return PROGRAM;
	if(val.compare("Fail")==0) return FAIL;
}
string enumToString(delta_t val)
{
	switch(val)
	{
		case BUFFER: return "Buffer";
		case CAMERA: return "Camera";
		case GEOOBJECT: return "GeoObject";
		case MESH: return "Mesh";
		case MODEL: return "Model";
		case SCENE: return "Scene";
		case OBJECT: return "Object";
		case PROGRAM: return "Program";
		default: return "Fail";
	}
}

/*.S.D.G.*/

#endif
