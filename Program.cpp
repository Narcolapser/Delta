#ifndef PROGRAM_CPP
#define PROGRAM_CPP
#include "Program.h"


Program::Program()
{//default contructor, everything is initially empty.
	newProg = 0;
	vs = 0;
	fs = 0;
	prog = 0;
}
Program::~Program()
{//ask GL to delete the respective objects.
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(prog);
}
Program::Program(xml_node self, string path)
{
	vs = create_shader((path + self.attribute("VertexShader").value()).c_str(),GL_VERTEX_SHADER);
	fs = create_shader((path + self.attribute("FragmentShader").value()).c_str(),GL_FRAGMENT_SHADER);
	updateProg();

	const char* uniform_name;
	uniform_name = self.attribute("VP").value();
	VP = glGetUniformLocation(prog, uniform_name);
	if (VP == -1) 
	{
		printf("Could not bind uniform %s\n", uniform_name);
	}
	uniform_name = self.attribute("local").value();
	local = glGetUniformLocation(prog, uniform_name);
	if (local == -1) 
	{
		printf("Could not bind uniform %s\n", uniform_name);
	}
}
//another trick like i used in the camera class. I'd hope that eventually I could remove 
//this function, but for now it remains to fill in the gaps where necessary.
int Program::loadVertex(const char* fileName)
{//loads a vertex shader using the function I borrowed from wiki books.
	vs = create_shader(fileName, GL_VERTEX_SHADER);
	if (vs) 
	{
		newProg = 0;
		return 1;
	}
	return 0;
}
int Program::loadFragment(const char* fileName)
{//loads a fragment shader using the function I borrowed from wiki books.
	fs = create_shader(fileName, GL_FRAGMENT_SHADER);
	if(fs) 
	{
		newProg = 0;
		return 1;
	}
	return 0;
}
void inline Program::use()
{//declares to openGl that this is the program that will be used right now. and will update
//the program if necessary.
	if (newProg==1)
	{
		glUseProgram(prog);
	}
	else
	{
		updateProg();
		glUseProgram(prog);
	}
}

//member functions:
GLuint Program::updateProg()
{//this method updates a program on the GPU, relinks and compiles it.
	prog = glCreateProgram();//create the new program
	if (vs == 0 || fs == 0) return 0;//if the shaders don't exist, back out now.
	glAttachShader(prog, vs);//attach the vertex shader
	glAttachShader(prog, fs);//attach the fragment shader/
		int link_ok;//sentinal value.
	glLinkProgram(prog);//link the program
	glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);//check the status of the program.
	if (!link_ok) //if it linked fine continue, other wise:
	{
		fprintf(stderr, "glLinkProgram:");// report the error
		print_log(prog);//print prog log for debugging.
		return 0;//exit
	}
	newProg = 1;//if it succeds the program is now at its most current version, set the
		//sentinal value to reflect such.
	return prog;//lastly return the resulting program.
}

//convenience methods: I copied these from wikibooks, I won't probably be maintaining them and as such I won't comment them.
//these may eventually get moved to the resource manager.
char* Program::file_read(const char* filename)
{
	FILE* in = fopen(filename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) {
		if (nb_read_total + BUFSIZ > res_size) {
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
GLint Program::create_shader(const char* filename, GLenum type)
{
	const GLchar* source = file_read(filename);
	if (source == NULL) 
	{
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
	glShaderSource(res, 1, &source, NULL);
	free((void*)source);
		glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) 
	{
		fprintf(stderr, "%s:", filename);
		print_log(res);
		glDeleteShader(res);
		return 0;
	}
	return res;
}
void Program::print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}
;
#endif
/*.S.D.G.*/
