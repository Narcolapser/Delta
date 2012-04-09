#include "Scene.h"

Scene::Scene(const char* config)
{
//	printf("Check point 1\n");
	char* inconfig = file_read(config);

	istringstream cs(inconfig,istringstream::in);
	string line;
	stack<GeoObject*> parenting;
	Mesh *temp;
	unsigned int parLevel = 0;
	unsigned int pltemp;
//	printf("Check point 2\n");
	while(getline(cs,line))
	{
		printf("%s\n",line.c_str());
		pltemp = (unsigned int)line.find_first_not_of("\t");
		line = line.substr(pltemp);
//		printf("Check point 3\n");

		string oFile;
//		printf("Check point 3.1\n");
		oFile = line.substr(0,line.find_first_of("\t"));
//		printf("Check point 3.2\n");
		line = line.substr(line.find("\t"));

//		printf("Check point 4\n");
		float trans[9];
		for(int i = 0; i < 9; i++)
		{
			trans[i] = atof(line.substr(0,line.find_first_of("\t")).c_str());
			line = line.substr(line.find_first_of('\t'));
//			printf("Check point 5.%i\n",i);
		}
//		pltemp = (unsigned int)oFile.find_first_not_of("\t");
//		oFile = oFile.substr(pltemp);

//		printf("Check point 6\n");
//		printf("Check point 7\n");
		printf("pltemp: %i parLevel: %i\n",pltemp,parLevel);
		if(pltemp > parLevel)
		{
			parenting.push(temp);
			parLevel = pltemp;
		}
		else if(pltemp < parLevel)
		{
			parenting.pop();
			pltemp = parLevel;
		}

		temp = new Mesh(oFile.c_str());
		temp->move(trans[0],trans[1],trans[2]);
		temp->rotate(trans[3],trans[4],trans[5]);

//		printf("parent level is: %i\n",pltemp);
//		printf("Check point 8\n");

		meshes.push_back(temp);
		if(!(parenting.empty()))
		{
			printf("Parenting baby!\n");
			temp->setParent(parenting.top());
		}
//		printf("Check point 9\n");
	}
}
void Scene::render()
{
	for(int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->render();
	}
}
void Scene::bindToProgram(Program *prog, GLint local)
{
	for(int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->bindToProg(prog,local);
	}
}
/*.S.D.G.*/
