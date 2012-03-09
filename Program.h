/* This class holds everything nescessary to deal with a GLSL program. which is:
 * Optional Geometry Shader.
 * Vertex Shader.
 * Fragment Shader.
 * It will hold this info as well as deal with loading, compiling, and linking.
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

class Program
{
public:
	Program()
	{
		newProg = 0;
		vs = 0;
		fs = 0;
		prog = 0;
	}
	~Program()
	{
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(prog);
	}	
	GLuint inline getProgram() { return newProg ? prog : updateProg(); }

//	void loadGeometry()
//	{
//		usingGeom = 1;		
//	}
	int loadVertex(const char* fileName)
	{
		vs = create_shader(fileName, GL_VERTEX_SHADER);
		if (vs) 
		{
			newProg = 0;
			return 1;
		}
		return 0;
	}
	int loadFragment(const char* fileName)
	{
		fs = create_shader(fileName, GL_FRAGMENT_SHADER);
		if(fs) 
		{
			newProg = 0;
			return 1;
		}
		return 0;
	}
	void inline use()
	{
		glUseProgram(prog);
	}
private:
	//data members:
	//int usingGeom;
	//GLuint gs, vs, fs;
	GLuint vs, fs;
	GLuint prog;
	int newProg;

	//member functions:
	GLuint updateProg()
	{
		prog = glCreateProgram();
		if (vs == 0 || fs == 0) return 0;
		glAttachShader(prog, vs);
		glAttachShader(prog, fs);

		int link_ok;
		glLinkProgram(prog);
		glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
		if (!link_ok) 
		{
			fprintf(stderr, "glLinkProgram:");
			print_log(prog);
			return 0;
		}
		newProg = 1;
		return prog;
	}

	//convenience methods:
	char* file_read(const char* filename)
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

	GLint create_shader(const char* filename, GLenum type)
	{
		const GLchar* source = file_read(filename);
		if (source == NULL) 
		{
			fprintf(stderr, "Error opening %s: ", filename); perror("");
			return 0;
		}
		GLuint res = glCreateShader(type);
		glShaderSource(res, 1, &source, NULL);
//		printf("Shader: %s \n",source);
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

	void print_log(GLuint object)
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
};

