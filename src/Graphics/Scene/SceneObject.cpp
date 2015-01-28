/* 
 * SceneObject.cpp
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

#include "SceneObject.h"

using namespace aergia::graphics::scene;

#include <iostream>
using namespace std;

SceneObject::SceneObject(){
	vData = NULL;
	indices = NULL;
	vbo = ind = 0;
	elementType = GL_TRIANGLES;
}

SceneObject::~SceneObject(){
	reset();
}

void SceneObject::reset(){
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ind) glDeleteBuffers(1, &ind);
	
	if(vData) delete vData;
	if(indices) delete indices;

	vData = NULL;
	indices = NULL;
	vbo = ind = 0;
	dataSize = 3;
}

bool SceneObject::init(){
	if(this->vData){
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, n_vData * dataSize * sizeof(GLfloat), this->vData, GL_STATIC_DRAW);
	}
	if(this->indices){
		glGenBuffers(1, &this->ind);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ind);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(GLuint), this->indices, GL_STATIC_DRAW);
	}
	return true;
}

void SceneObject::draw(unsigned int mode){	
	if(!this->vData || !this->indices || !this->vbo || !this->ind) return;
	
	//this->shader.begin();

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ind);

	glEnableVertexAttribArray(0);                                               //ENABLE VERTEX POSITION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);      // VERTEX POSITION POINTER

	glDrawElements(elementType, this->n_indices, GL_UNSIGNED_INT, 0);

	//this->shader.end();
}

void SceneObject::apply(Transform t){}
