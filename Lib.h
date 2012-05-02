#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>

#include "lib/pugixml/pugixml.cpp"

#ifndef LIB_H
#define LIB_H

using namespace std;

typedef int UID;
enum variant_t
{
	TYPE_BOOL,
	TYPE_CHAR,
	TYPE_SHORT,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_LONG,
	TYPE_DOUBLE,
	TYPE_COUNT//a trick I got from Jason Gregory. this enum counts the number 
			//of unique types. clever no?
};

typedef struct variant
{
	variant(char vals[16])
	{
		for(int i = 16; i-->0;datum.v_asChar[i] = vals[i]);
	}
	variant(variant& val)
	{
		datum.v_asLong[0] = val.datum.v_asLong[0];
		datum.v_asLong[1] = val.datum.v_asLong[1];
	}
	variant(int val0, int val1, int val2, int val3)
	{
		datum.v_asInt[0] = val0;
		datum.v_asInt[1] = val1;
		datum.v_asInt[2] = val2;
		datum.v_asInt[3] = val3;
	}
	variant(float val0, float val1, float val2, float val3)
	{
		datum.v_asFloat[0] = val0;
		datum.v_asFloat[1] = val1;
		datum.v_asFloat[2] = val2;
		datum.v_asFloat[3] = val3;
	}
	variant()
	{
		datum.v_asLong[0] = 0l;
		datum.v_asLong[1] = 0l;
	}
	variant_t type;
	union
	{
		bool v_asBool[ARG_SIZE/sizeof(bool)];
		char v_asChar[ARG_SIZE/sizeof(char)];
		short v_asShort[ARG_SIZE/sizeof(short)];
		int v_asInt[ARG_SIZE/sizeof(int)];
		float v_asFloat[ARG_SIZE/sizeof(float)];
		long int v_asLong[ARG_SIZE/sizeof(long int)];
		double v_asDouble[ARG_SIZE/sizeof(double)];
	} datum;
}Variant;

enum delta_t {FAIL,OBJECT,BUFFER,CAMERA,GEOOBJECT,MESH,MODEL,SCENE,PROGRAM,EVENT};

/*.S.D.G.*/
#endif
