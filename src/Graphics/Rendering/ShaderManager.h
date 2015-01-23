/* 
 * ShaderManager.h
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

#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <OpenGL.h>
#include <Config.h>
using aergia::common::Config;

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

namespace aergia {

	namespace graphics {

		namespace rendering {

#define printOpenGLError() printOglError(__FILE__, __LINE__)

			class ShaderManager {
				private:
					ShaderManager();
				public:
					static ShaderManager& getInstance(){
						static ShaderManager sm;
						return sm;
					}

					int load(const char* name);
                    int loadFromFiles(char const *name, char const *path);

					int useShader(GLuint program);

					int getProgramID(const char* name);

					const char* getVertexShaderSource(string v);
					const char* getFragmentShaderSource(string f);
					vector<GLuint> programs;

				private:
					map<string,GLuint> name_program;
					map<string,GLuint> name_defaultProgram;

					map<string,const char*> vertexShadersSources;
					map<string,const char*> fragmentShadersSources;

					map<string,const char*> _vertexShadersSources;
					map<string,const char*> _fragmentShadersSources;

					void loadDefaultShaders();

					// SHADERS INSTALLATION
					bool init();

					GLuint createProgram(const GLchar*, const GLchar*);
					int readShader(const char *fileName, char **shaderText);
				    GLuint compile(const char* shaderSource, GLuint shaderType);
                    GLuint createProgram(GLuint objects[], int size);

					// debug
					int printOglError(const char *file, int line);
					void printShaderInfoLog(GLuint shader);
					void printProgramInfoLog(GLuint program);
					void getGlVersion( int *major, int *minor );


            };
		}
	}
}

#endif
