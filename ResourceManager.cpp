#include "ResourceManager.h"

#ifndef RESOURCEMANAGER_CPP
#define RESOURCEMANAGER_CPP
ResourceManager::ResourceManager()
{
	IDc = 0;
	freeCount = 1;
}
ResourceManager::~ResourceManager()
{
	IDc = 0;
	freeCount = -1;
}
UID ResourceManager::RequestID()
{
	++IDc;
	Lease *temp = new Lease(IDc);
	leases.push_back(temp);
	return(IDc);
}
bool ResourceManager::AssignID(UID val, Object* foo)
{
	int idLoc =  findLease(val);
	if(idLoc == -1) return false;
	Lease *temp = leases[idLoc];
	if(temp->refCount != 0) return false;
	temp->pointer = foo;
	++(temp->refCount);
	return true;
}
bool ResourceManager::RetainID(UID val)
{
	int idLoc = findLease(val);
	if(idLoc == -1) return false;
	Lease *temp = leases[idLoc];
	if(temp->refCount == 0) return false;
	++(temp->refCount);
	return true;
}
Object* ResourceManager::GetIDRetaining(UID val)
{
	int idLoc = findLease(val);
	if(idLoc == -1) return NULL;
	Lease *temp = leases[idLoc];
	if(temp->refCount == 0) return NULL;
	++(temp->refCount);
	return temp->pointer;
}
Object* ResourceManager::GetIDNonRetaining(UID val)
{
	int idLoc = findLease(val);
	if(idLoc == -1) return NULL;
	Lease *temp = leases[idLoc];
	if(temp->refCount == 0) return NULL;
	return temp->pointer;
}
void ResourceManager::Release(UID val)
{
	int idLoc = findLease(val);
	if(idLoc == -1) return;
	Lease *temp = leases[idLoc];
	if(temp->refCount == 0) return;
}
int ResourceManager::findLease(UID val)
{
	int high = val;
	int low = val - freeCount;
	int mid;
	for(mid = low + ((high - low)/2);mid != low && leases[mid]->ID != val;)
	{
		if(leases[mid]->ID > val) low = mid;
		else if(leases[mid]->ID < val) high = mid;
		else return mid;
		mid = low + ((high - low)/2);
	}
	if(leases[mid]->ID == val) return mid;
	return -1;
}
UID ResourceManager::isDuplicate(const char* val)
{
	printf("%s shows up %i times\n",val,(int)loaded.count(val));
	if(loaded.count(val)==0) return false;//place holder function, i'll flush this out later.
	printf("%s has the ID of: %i\n",val,loaded[val]);
	return loaded[val];
}
UID ResourceManager::LoadMesh(const char* filename)
{
	UID temp = isDuplicate(filename);
	if(temp != 0) return temp;
	Mesh* val = new Mesh(filename);
	temp = RequestID();
	val->assignID(temp);
	if(AssignID(temp,val))
	{
		loaded[filename] = temp;
		return temp;
	}
	delete val;
	return 0;
}
ResourceManager::Lease::Lease(UID val, Object* foo, unsigned short int bar)
{
	ID = val;
	pointer = foo;
	refCount = bar;
}
ResourceManager::Lease::Lease(UID val)
{
	ID = val;
	pointer = NULL;
	refCount = 0;
}
ResourceManager::Lease::~Lease()
{
	ID = 0;
	pointer = NULL;
	refCount = 0;
}

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
		default: return "Fail";
	}
}
#endif
/*.S.D.G.*/
