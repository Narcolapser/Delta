#include "Scene.h"

Scene::Scene(const char* config)
{
//	printf("Check point 1\n");
	char* inconfig = file_read(config);

/*	xml_document doc;
	xml_parse_result result;
	if(isFile)
	{
		result = doc.load_file(config);
	}
	else
	{
		result = doc.load(config);
	}*/
	istringstream cs(inconfig,istringstream::in);
	string line;

	stack<GeoObject*> parenting;
	Model *temp;
	UID meshUID;
	unsigned int parLevel = 0;
	unsigned int pltemp;
//	printf("Check point 2\n");
	while(getline(cs,line))
	{
//		printf("%s\n",line.c_str());
		pltemp = (unsigned int)line.find_first_not_of("\t");
		line = line.substr(pltemp);
//		printf("Check point 3\n");

		string oFile;
//		printf("Check point 3.1\n");
		oFile = line.substr(0,line.find_first_of("\t"));
//		printf("Check point 3.2\n");
		line = line.substr(line.find("\t")+1);

//		printf("Check point 4\n");
		float trans[9];
		for(int i = 0; i < 9; i++)
		{
			trans[i] = atof(line.substr(0,line.find_first_of("\t")).c_str());
//			printf("out of %s comes: %f\n",line.substr(0,line.find_first_of("\t")).c_str(),trans[i]);
			line = line.substr(line.find_first_of('\t')+1);
//			printf("Check point 5.%i\n",i);
		}
//		pltemp = (unsigned int)oFile.find_first_not_of("\t");
//		oFile = oFile.substr(pltemp);


//		printf("pltemp: %i parLevel: %i\n",pltemp,parLevel);
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

//		temp = new Mesh(oFile.c_str());
//		printf("Check point 6\n");
		meshUID = globalRM->LoadMesh(oFile.c_str());
//		printf("Check point 6.5, ID of: %i\n",tempUID);
		temp = new Model(meshUID);
//		temp = (Mesh*)globalRM->GetIDNonRetaining(tempUID);
//		printf("Check point 7, pointer to: %li\n\n",(long int)temp);
		temp->move(trans[0],trans[1],trans[2]);
//		printf("x: %f y: %f z: %f",trans[0],trans[1],trans[2]);
		temp->rotate(trans[3],trans[4],trans[5]);

//		printf("parent level is: %i\n",pltemp);
//		printf("Check point 8\n");

		models.push_back(temp);
		if(!(parenting.empty()))
		{
//			printf("Parenting baby!\n");
			temp->setParent(parenting.top());
		}
//		printf("Check point 9\n");
	}
}
Scene::Scene(xml_node config)
{
	string path = config.attribute("path").value() + string("/");
	printf("The path is: %s\n",path.c_str());
	for(xml_node i = config.first_child(); i; i = i.next_sibling())
	{
		load(i,NULL,path);
	}
}

void Scene::load(xml_node self, GeoObject* parent, string path)
{
	delta_t name = stringToEnum(self.name());
	printf("node to load: %s valued at: %i\n",self.name(),name);
	GeoObject* temp;
	switch(name)
	{
	case MODEL:
		temp = new Model(self,path);
		models.push_back((Model*)temp);
		break;
	default:
		printf("Defaulted! %s\n",enumToString(name).c_str());
		break;
	}
	temp->setParent(parent);
	for(xml_node i = self.first_child(); i; i = i.next_sibling())
	{
		load(i,temp,path);
	}
}

void Scene::render()
{
	for(int i = 0; i < models.size(); i++)
	{
//		printf("rendering %i\n",i);
		models[i]->render();
//		printf("Pointer: %li\n",(long int)models[i]);
	}
}
void Scene::bindToProgram(Program *prog, GLint local)
{
	for(int i = 0; i < models.size(); i++)
	{
//		printf("Pointer: %li\n",(long int)models[i]);
		models[i]->bindToProgram(prog,local);
	}
}
/*.S.D.G.*/
