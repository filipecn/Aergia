/* 
 * Shapes.cpp
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

#include "Shapes.h"
using namespace aergia::graphics::helpers;

#include <iostream>
using namespace std;

Rectangle::Rectangle(vec4 _bbox){
	this->bbox[0] = vec3(_bbox.x,_bbox.y,0.0);
	this->bbox[1] = vec3(_bbox.z,_bbox.w,0.0);
	pos.x = bbox[0].x;
	pos.y = bbox[0].y;
}

void Rectangle::draw(unsigned int mode){
	if(!this->vData || !this->indices || !this->vbo || !this->ind) return;
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	
	texture.apply();

	shader.start();
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ind);
	glDrawElements(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, 0);

	shader.stop();
}

Box::Box(vec3 center, vec3 size){
	this->center = center;
	this->size = size;
	
	bbox[0] = vec3( center.x - size.x / 2.0,
			center.y - size.y / 2.0,
			center.z - size.z / 2.0);

	bbox[1] = vec3( center.x + size.x / 2.0,
			center.y + size.y / 2.0,
			center.z + size.z / 2.0);
	cout << bbox[0] << bbox[1] << endl;
	dataSize = 3;
	n_vData = 8;
	vData = new GLfloat[n_vData*dataSize];
	//   7__________6 
	//  / |       / |
	//4/__|______/5 |
	// | 3|______|__| 2
	// |  /      |  /
	// | /       | /
	// |/________|/
	// 0          1
	vData[0*3+0] = bbox[0].x; vData[0*3+1] = bbox[0].y; vData[0*3+2] = bbox[0].z;
	vData[1*3+0] = bbox[1].x; vData[1*3+1] = bbox[0].y; vData[1*3+2] = bbox[0].z;
	vData[2*3+0] = bbox[1].x; vData[2*3+1] = bbox[0].y; vData[2*3+2] = bbox[1].z;
	vData[3*3+0] = bbox[0].x; vData[3*3+1] = bbox[0].y; vData[3*3+2] = bbox[1].z;
	vData[4*3+0] = bbox[0].x; vData[4*3+1] = bbox[1].y; vData[4*3+2] = bbox[0].z;
	vData[5*3+0] = bbox[1].x; vData[5*3+1] = bbox[1].y; vData[5*3+2] = bbox[0].z;
	vData[6*3+0] = bbox[1].x; vData[6*3+1] = bbox[1].y; vData[6*3+2] = bbox[1].z;
	vData[7*3+0] = bbox[0].x; vData[7*3+1] = bbox[1].y; vData[7*3+2] = bbox[1].z;

	n_indices = 12*3;
	indices = new GLuint[n_indices];
	indices[0*3+0] = 0; indices[0*3+1] = 3; indices[0*3+2] = 1;
	indices[1*3+0] = 1; indices[1*3+1] = 3; indices[1*3+2] = 2;
	indices[2*3+0] = 4; indices[2*3+1] = 5; indices[2*3+2] = 7;
	indices[3*3+0] = 7; indices[3*3+1] = 5; indices[3*3+2] = 6;
	indices[4*3+0] = 0; indices[4*3+1] = 5; indices[4*3+2] = 4;
	indices[5*3+0] = 0; indices[5*3+1] = 1; indices[5*3+2] = 5;
	indices[6*3+0] = 3; indices[6*3+1] = 7; indices[6*3+2] = 6;
	indices[7*3+0] = 3; indices[7*3+1] = 6; indices[7*3+2] = 2;
	indices[8*3+0] = 0; indices[8*3+1] = 4; indices[8*3+2] = 3;
	indices[9*3+0] = 3; indices[9*3+1] = 4; indices[9*3+2] = 7;
	indices[10*3+0]= 1; indices[10*3+1]= 2; indices[10*3+2]= 5;
	indices[11*3+0]= 5; indices[11*3+1]= 2; indices[11*3+2]= 6;
	
	init();
}

void Box::draw(unsigned int mode){
	if(!this->vData || !this->indices || !this->vbo || !this->ind) return;
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	
	
	//texture.apply();

	//shader.start();
	glColor3fv(&color[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ind);
	glEnableVertexAttribArray(0);                                               //ENABLE VERTEX POSITION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);      // VERTEX POSITION POINTER
	glDrawElements(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, 0);

	//shader.stop();
}


