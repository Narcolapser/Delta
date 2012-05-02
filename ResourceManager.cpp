#ifndef RESOURCEMANAGER_CPP
#define RESOURCEMANAGER_CPP
#include "ResourceManager.h"
ResourceManager::ResourceManager()
{//the constructor. not much to construct. just need to initalize the local variables.
	IDc = 0;
	freeCount = 1;
}
ResourceManager::~ResourceManager()
{//clear out them local variables!
	IDc = 0;
	freeCount = -1;
}
UID ResourceManager::RequestID()
{//This method is used to get an ID. I'll probably have to make this thread safe later, but i'm not
	//going to concern myself with threads for now.
	//increment the ID number. this makes sure that 0 will never be an ID.
	++IDc;

	//create a new lease for that that ID. 
	Lease *temp = new Lease(IDc);

	//push that lease into the vector.
	leases.push_back(temp);

	//return the new ID
	return(IDc);
}
bool ResourceManager::AssignID(UID val, Object* foo)
{//this method is used to assign a requested ID to a given object. if the assignment succeeds then
	//it will return true, if it fails in anyway, it will return false. best plan at that 
	//point is to just ask for a new ID, they are cheap.

	//find the ID in question amongst the leases
	int idLoc =  findLease(val);

	//if the lease does not exist, there is no way it can be assigned with certainty that the
	//resource manager will know. return false
	if(idLoc == -1) return false;

	//since there seems to be a lease, get a pointer to the lease.
	Lease *temp = leases[idLoc];

	//if the lease has a refcount greater than 0, then it can't be used as it has already been
	//assigned to a different object. return false.
	if(temp->refCount != 0) return false;

	//so the lease exists and isn't already in use. The lease is then granted, save the 
	//reference to the object that now owns the lease in the lease.
	temp->pointer = foo;

	//increment the reference counter on the lease to reflect that the lease now has an owner.
	++(temp->refCount);

	//assignment successful.
	return true;
}
bool ResourceManager::RetainID(UID val)
{//this method tells the resource manager that some one else is now has interest in this object.
	//the difference here from AssignID is that if refCount == 0 this method will fail, this
	//method does not exist to be the assigner of the object, only to make certain its
	//persistance beyond the inital user declaring it doesn't need it.

	//find the lease in question.
	int idLoc = findLease(val);

	//if the lease does not exist, there is no way it could be incremented. return false.
	if(idLoc == -1) return false;

	//so the lease exists. fetch pointer to said lease.
	Lease *temp = leases[idLoc];

	//if the reference count is 0, the lease has not been assigned, and therefore cannot be
	//retained, as there is nothing to retain.
	if(temp->refCount == 0) return false;

	//increment the refCount to reflect the extra interest.
	++(temp->refCount);

	//retenation successful.
	return true;
}
Object* ResourceManager::GetIDRetaining(UID val)
{//these two methods are as the sound, both will return a pointer to the object of the lease
	//you request. but one will increase the ref count, one won't. there are valid uses for
	//both cases, and so both methods exist.
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
{//this does the opposite of retain. it frees up a lease. if this takes the refcount to 0,
	//it also then deals with the garbage colleting.
	int idLoc = findLease(val);
	if(idLoc == -1) return;
	Lease *temp = leases[idLoc];
	if(temp->refCount == 0) return;
}
int ResourceManager::findLease(UID val)
{
	//this is a simple binary search for the lease requested by UID. Because I can depend on
	//vector keeping things in chronological order, and I know that the ID's are given out
	//sequentially, I know that the object isn't going to be any higher than its ID number.
	//further, it can only be as low as the number of releases that have happened. this means
	//that the lowest possible location is the ID - the freeCount. If there were 1m objects
	//this would take 20 searches with out this trick. with this trick the number can be
	//as low as 1 if there have been now releases, with a probable case of like 6-8. woohoo!
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
{//checks to see if the requested load is a duplicate. if it is a duplicate, then don't load it!
	//just simply send them the one that already exists.

	//this functionality is yet to come truth be told. but the connections are all in place!
	if(loaded.count(val)==0) return false;//place holder function, i'll flush this out later.
	return loaded[val];
}
UID ResourceManager::LoadMesh(const char* filename)
{//load a Mesh object. this pulls in a .obj file and does the nescessary resource manageing on it.

	//first check for duplication.
	UID temp = isDuplicate(filename);

	//if it is a duplicate, fantastic, return the object it is already loaded, we are done.
	if(temp != 0) return temp;

	//if it isn't a duplicate, create the new Mesh.
	Mesh* val = new Mesh(filename);

	//get a new ID for this mesh.
	temp = RequestID();

	//assign the ID to the mesh.
	val->assignID(temp);

	if(AssignID(temp,val))
	{//if the assignment succeeds, then return the newly loaded mesh.
		loaded[filename] = temp;
		return temp;
	}

	//if it fails. deallocate it and return 0
	delete val;
	return 0;
}
ResourceManager::Lease::Lease(UID val, Object* foo, unsigned short int bar)
{//constructor for a lease where all values are know for the begining.
	ID = val;
	pointer = foo;
	refCount = bar;
}
ResourceManager::Lease::Lease(UID val)
{//constructor for a lease when only the Identifier is known
	ID = val;
	pointer = NULL;
	refCount = 0;
}
ResourceManager::Lease::~Lease()
{//destructor for a lease.
	ID = 0;
	pointer = NULL;
	refCount = 0;
}

//convenience methods, they do what it sounds like and work just fine. they are very small in scope
//so i'm very confident that if a bug is ever below this point, i just added it. there isn't a lot
//that could go wrong down here.
#endif
/*.S.D.G.*/
