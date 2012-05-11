/* This class's purpose is to deal with the loading and deletion of files for the rest of the
 * program. In the future this will allow me to make redundancy checks and that sort of thing. But
 * for now, I've got this as a small first step. 
 */

#include "Interface.cpp"
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//RESOURCE MANAGER! ONE INCLUDE TO RULE THEM ALL!!!
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.cpp"
#include "Mesh.cpp"
#include "Buffer.cpp"

using namespace std;
using namespace pugi;

struct delayedRequest
{
	delayedRequest()
	{
		callBack = NULL;
		LTI = "";
		recv = (UID)0;
		funcID = -1;
	}
	delayedRequest(delayedRequest& val)
	{
		callBack = val.callBack;
		LTI = val.LTI;
		recv = val.recv;
		funcID = val.funcID;
	}
	delayedRequest(const delayedRequest& val)
	{
		callBack = val.callBack;
		LTI = val.LTI;
		recv = val.recv;
		funcID = val.funcID;
	}
	void (*callBack)(const delayedRequest& val);
	string LTI;
	UID recv;
	int funcID;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	UID RequestID();
	bool AssignID(UID val, Object* foo);
	bool RetainID(UID val);
	void Release(UID val);
	UID LoadMesh(const char* filename);
	Object* GetIDRetaining(UID val);
	Object* GetIDNonRetaining(UID val);
	virtual bool onEvent(const Event& event);
	bool RegisterLTI(string val, UID bar);
	UID ResolveLTI(string val);
	void RegisterRequest(const Event& val);
	void ResolveRequests();
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
	map<string,UID> loaded;
	map<string,UID> LTIReg;
	vector<Event> DeReqs;
	int IDc;
	int freeCount;

	UID isDuplicate(const char* val);
	void freeLease(UID val);
	int findLease(UID val);
};
static ResourceManager* globalRM;
#endif
/*.S.D.G.*/
