#include "ResourceManager.h"
//typedef unsigned int UID;
//ResourceManager::ResourceManager(const char* config, bool isFile)
//{
//	xml_document doc;
//	xml_parse_result result;
//	if(isFile)
//	{
//		result = doc.load_file(config);
//	}
//	else
//	{
//		result = doc.load(config);
//	}
//	cout << result << endl;
//	
//	xml_node scene = doc.child("Scene");
//	cout << scene.attribute("name").value() << endl;
//	cout << scene.child().attribute("file").value() << endl;
//}
ResourceManager::ResourceManager()
{
	IDc = 0;
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
	unsigned int high = val;
	unsigned int low = val - freeCount;
	unsigned int mid;
	for(mid = low + ((high - low)/2); 
		mid != low && leases[mid]->ID != val;)
	{
		if(leases[mid]->ID > val) low = mid;
		else if(leases[mid]->ID < val) high = mid;
		else return mid;
		mid = low + ((high - low)/2);
	}
	if(leases[mid]->ID == val) return mid;
	return -1;
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
