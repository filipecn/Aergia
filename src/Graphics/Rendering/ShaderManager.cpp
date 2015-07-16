/* 
 * ShaderManager.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 FilipeCN
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "ShaderManager.h"
using namespace aergia::graphics::rendering;

#include <iostream>
using namespace std;

ShaderManager::ShaderManager(){
	init();
	if(Config::status(A_DEFAULT)){
		loadDefaultShaders();
	}
}

int ShaderManager::load(const char *name){
	cout << "LOAD SHADER " << name << endl;
	auto it = name_program.find(string(name));
	if(it != name_program.end())
		return it->second;
	
	string vertexShaderName = string(name) + "_v";
	string fragmentShaderName = string(name) + "_f";
	
	const char *vertexSource = getVertexShaderSource(vertexShaderName);
	if(!vertexSource){
		return 0;
	}
	const char *fragmentSource = getFragmentShaderSource(fragmentShaderName);
	if(!fragmentSource)
		return 0;

	int newProgram = createProgram(vertexSource,fragmentSource);
	
	if(!newProgram)
		return 0;
	
	name_program[string(name)] = (unsigned int) programs.size();
	vertexShadersSources[vertexShaderName] = vertexSource;
	fragmentShadersSources[fragmentShaderName] = fragmentSource;

	programs.push_back(newProgram);

	return programs.size()-1;
}

int ShaderManager::useShader(GLuint id){
	if(id >= programs.size() || !programs[id])
		return 0;
	glUseProgram(programs[id]);
	return programs[id];
}

int ShaderManager::getProgramID(const char *name){
	auto it = name_program.find(string(name));
	if(it == name_program.end()){
	//	int newID = load(name);
	///*	if(newID < 0){
			if(Config::getInstance().status(A_DEFAULT)){
				auto def = name_defaultProgram.find(string(name));
				if(def == name_defaultProgram.end())
					return -1;
				cout << "GOT FROM DEFAULT!\n";
				return def->second;
			}
			return -1;
	//	}*/
	//	return newID;
	}
	return it->second;
}

const char* ShaderManager::getVertexShaderSource(string v){
	auto it = vertexShadersSources.find(v);
	if(it == vertexShadersSources.end()){
		auto def = _vertexShadersSources.find(v);
		//if(def != _vertexShadersSources.end())
		//	return def->second;
		return NULL;
	}
	return it->second;
}

const char* ShaderManager::getFragmentShaderSource(string f){
	auto it = fragmentShadersSources.find(f);
	if(it == fragmentShadersSources.end()){
		auto def = _fragmentShadersSources.find(f);
		//if(def != _fragmentShadersSources.end())
		//	return def->second;
		return NULL;
	}
	return it->second;
}

int ShaderManager::loadFromFiles(const char *name, const char *path){
    string base(path);
    if(base[base.length()-1] != '/')
        base += '/';
    base += name;

    GLuint objects[] = {0,0,0};
    string ext[] = {string(".vs"),string(".gs"),string(".fs")};
    GLuint types[] = {GL_VERTEX_SHADER,GL_GEOMETRY_SHADER,GL_FRAGMENT_SHADER};
    for (int i = 0; i < 3; ++i) {
        string file = base + ext[i];
        char *source = NULL;
        if(readShader(file.c_str(), &source)) {
            cout << "Compiling " << file << endl;
            objects[i] = compile(source, types[i]);
            free(source);
        }
    }

    GLuint program = createProgram(objects, 3);
    if(!program)
        return -1;
    name_program[string(name)] = (unsigned int) programs.size();
    programs.push_back(program);
    return (int) (programs.size() - 1);
}

void ShaderManager::loadDefaultShaders(){
	_vertexShadersSources[string("texture_v")] = 
		"#version 150 core\n"
		"in vec2 position;"
//		"in vec3 color;"
		"in vec2 texcoord;"
//		"out vec3 Color;"
		"out vec2 Texcoord;"
		"void main() {"
//		"   Color = color;"
		"   Texcoord = texcoord;"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"}";

	_fragmentShadersSources[string("texture_f")] = 
		"#version 150 core\n"
//		"in vec3 Color;"
		"in vec2 Texcoord;"
		"out vec4 outColor;"
		"uniform sampler2D texKitten;"
		"void main() {"
		"   outColor = texture(texKitten, Texcoord);"
	//	"   outColor = vec4(Texcoord,0,1);"
		"}";

	name_defaultProgram[string("texture")] = programs.size();
	programs.push_back(createProgram(_vertexShadersSources[string("texture_v")],
				_fragmentShadersSources[string("texture_f")]));
}

///////////////////////// SHADERS INSTALLATION /////////////////////////////////

// Print out the information log for a shader object
void ShaderManager::printShaderInfoLog(GLuint shader){
	GLsizei infologLength = 0;
	int charsWritten  = 0;
	GLchar *infoLog;

    CHECK_GL_ERRORS;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

    CHECK_GL_ERRORS;

	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc((size_t) infologLength);
		if (infoLog == NULL)
		{
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("Shader InfoLog:\n%s\n\n", infoLog);
		free(infoLog);
		if(charsWritten){
			exit(1);
		}
	}
    CHECK_GL_ERRORS;
}

// Print out the information log for a program object
void ShaderManager::printProgramInfoLog(GLuint program){
	int infologLength = 0;
	int charsWritten  = 0;
	GLchar *infoLog;

    CHECK_GL_ERRORS;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);

    CHECK_GL_ERRORS;

	if (infologLength > 0)
	{
		infoLog = (GLchar *)malloc(infologLength);
		if (infoLog == NULL)
		{
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
		printf("Program InfoLog:\n%s\n\n", infoLog);
		free(infoLog);
	}
    CHECK_GL_ERRORS;
}

int ShaderManager::readShader(const char *fileName, char **shaderText){
	int count;

    int fd = open(fileName, O_RDONLY);
	if (fd == -1)
		return 0;

    size_t size = (size_t) (lseek(fd, 0, SEEK_END) + 1);
	close(fd);
	*shaderText = new char[size];

	FILE *f = fopen(fileName, "r");
	if(!f)
		return 0;

	fseek(f, 0, SEEK_SET);
	count = (int) fread(*shaderText, 1, size, f);
    (*shaderText)[count] = '\0';

	if (ferror(f))
		count = 0;

	fclose(f);
	return count;
}

int ShaderManager::printOglError(const char *file, int line){
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void ShaderManager::getGlVersion(int *major, int *minor){
	const char* str = (const char*)glGetString( GL_VERSION );
	if( (str == NULL) || (sscanf(str, "%d.%d", major, minor ) != 2) )
	{
		*major = *minor = 0;
		fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
	}
}

bool ShaderManager::init(){
	bool success = false;
	int gl_major, gl_minor;

	// Initialize the "OpenGL Extension Wrangler" library
	glewInit();

	// Make sure that OpenGL 2.0 is supported by the driver
	getGlVersion(&gl_major, &gl_minor);
	printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);

	if (gl_major < 2)
	{
		printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
		printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
		exit(1);
	}

	return success;
}

GLuint ShaderManager::createProgram(const GLchar *vertexShaderSource, const GLchar *fragmentShaderSource){
	GLuint ProgramObject;   // handles to objects
	GLint  vertCompiled, fragCompiled;    // status values
	GLint linked;

	// Create a vertex shader object and a fragment shader object
	//
	GLuint VertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	//
	// Load source code strings into shaders
	//
	glShaderSource(VertexShaderObject, 1, &vertexShaderSource, NULL);
	glShaderSource(FragmentShaderObject, 1, &fragmentShaderSource, NULL);
	//
	// Compile the brick vertex shader, and print out
	// the compiler log file.
	//
	glCompileShader(VertexShaderObject);
    CHECK_GL_ERRORS;
	glGetShaderiv(VertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);
	printShaderInfoLog(VertexShaderObject);
	//
	// Compile the brick vertex shader, and print out
	// the compiler log file.
	//
	glCompileShader(FragmentShaderObject);
    CHECK_GL_ERRORS;
	glGetShaderiv(FragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);
	printShaderInfoLog(FragmentShaderObject);
	if (!vertCompiled || !fragCompiled)
		return 0;

	// Create a program object and attach the two compiled shaders
	//
	ProgramObject = glCreateProgram();
	glAttachShader(ProgramObject, VertexShaderObject);
	glAttachShader(ProgramObject, FragmentShaderObject);

	// Link the program object and print out the info log
	//
	glLinkProgram(ProgramObject);
    CHECK_GL_ERRORS;
	glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
	printProgramInfoLog(ProgramObject);
	if (!linked)
		return 0;
	return ProgramObject;
}

GLuint ShaderManager::compile(const char *shaderSource, GLuint shaderType) {
    GLint  compiled;
    GLuint shaderObject = glCreateShader(shaderType);

    glShaderSource(shaderObject, 1, &shaderSource, NULL);

    glCompileShader(shaderObject);
    CHECK_GL_ERRORS;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
    printShaderInfoLog(shaderObject);

    return shaderObject;
}

GLuint ShaderManager::createProgram(GLuint objects[], int size) {
    GLuint programObject = glCreateProgram();

    for (int i = 0; i < size; ++i)
        if(objects[i])
            glAttachShader(programObject, objects[i]);

    glLinkProgram(programObject);
    CHECK_GL_ERRORS;
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    printProgramInfoLog(programObject);
    if (!linked)
        return 0;
    return programObject;
}
