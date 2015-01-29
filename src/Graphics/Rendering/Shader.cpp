/* 
 * Shader.cpp
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

#include "Shader.h"

using namespace aergia::graphics::rendering;

#include <iostream>
using namespace std;

Shader::Shader(){
    running = false;
}

Shader::Shader(const char* name){
	strcpy(this->name, name);
	id = ShaderManager::getInstance().getProgramID(name);
    running = false;
}

bool Shader::set(const char* name){
	strcpy(this->name, name);
	id = ShaderManager::getInstance().getProgramID(name);
    running = false;
	return id != 0;
}

bool Shader::loadFiles(const char *name, const char *path) {
    strcpy(this->name, name);
    id = ShaderManager::getInstance().loadFromFiles(name, path);
    running = false;
    return false;
}

bool Shader::begin(){
    if(running)
        return true;
	int shaderProgram = ShaderManager::getInstance().useShader(id);
	if(!shaderProgram)
		return false;
	for(auto va : vertexAttributes){
		GLint attribute = glGetAttribLocation(shaderProgram, va.name);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, va.size, GL_FLOAT, GL_FALSE, vDataSize * sizeof(GLfloat), (void*)(va.offset * sizeof(GLfloat)));
	}
    running = true;
	return true;
}

void Shader::end(){
	glUseProgram(0);
    running = false;
}

void Shader::addVertexAttribute(const char *attribute, int size, int offset){
	VertexAttribute va;
	va.name = attribute;
	va.size = size;
	va.offset = offset;

	vertexAttributes.push_back(va);
}

void Shader::setUniform(const char* name, mat4 m){
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char* name, mat3 m){
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char* name, vec4 m){
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniform4fv(loc, 1, &m[0]);
}

void Shader::setUniform(const char* name, vec3 m){
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniform3fv(loc, 1, &m[0]);
}

void Shader::setUniform(const char* name, vec2 m){
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniform4fv(loc, 1, &m[0]);
}

void Shader::setUniform(const char* name, int i){
    bool wasNotRunning = !running;
	GLint loc = getUniLoc(name);
	if(loc == -1)
		return;
	glUniform1i(loc, i);
    if(wasNotRunning)
        end();
}

void Shader::setUniform(const char* name, float f){
    bool wasNotRunning = !running;
    GLint loc = getUniLoc(name);
    if(loc == -1)
        return;
    glUniform1i(loc, f);
    if(wasNotRunning)
        end();
}

GLint Shader::getUniLoc(const GLchar *name){
	int shaderProgram = ShaderManager::getInstance().useShader(id);
	if(!shaderProgram)
		return -1;
	return glGetUniformLocation(shaderProgram, name);
}


