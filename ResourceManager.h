/* This class's purpose is to deal with the loading and deletion of files for the rest of the
 * program. In the future this will allow me to make redundancy checks and that sort of thing. But
 * for now, I've got this as a small first step. 
 * This class contains:
 */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>

#include "Object.h"
//typedef unsigned int UID;

//#include "lib/pugixml/pugixml.cpp"

using namespace std;
//using namespace pugi;

class ResourceManager
{
public:
	ResourceManager();
//	~ResourceManager();
	UID RequestID();
	bool AssignID(UID val, Object* foo);
	bool RetainID(UID val);
	void Release(UID val);
	Object* GetIDRetaining(UID val);
	Object* GetIDNonRetaining(UID val);
private:
	struct Lease
	{
		UID ID;
		Object* pointer;
		unsigned short int refCount;
		Lease(UID val, Object* foo, unsigned short int bar);
		Lease(UID val);
		~Lease();
	};
	vector<Lease*> leases;
	unsigned int IDc;
	unsigned int freeCount;
	void freeLease(UID val);
	int findLease(UID val);
};

#endif
/*.S.D.G.*/
