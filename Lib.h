#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>

#include "lib/pugixml/pugixml.cpp"


//#define DEBUGLEVEL1 printf("

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
