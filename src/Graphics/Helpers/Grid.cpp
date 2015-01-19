/* 
 * Grid.cpp
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

#include "Grid.h"

using namespace aergia::graphics::helpers;

Grid::Grid(){
}

Grid::Grid(int n){
	this->nx = this->ny = this->nz = n;
}

Grid::~Grid(){

}

void Grid::draw(unsigned int mode){
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	for(int i = -nx; i <= nx; i++){
		glColor3f(1,1,1);
		if(!i) glColor3f(0,0,1);
		glVertex3f(i, -ny, 0);
		glVertex3f(i, ny, 0);
		if(!i) glColor3f(0,1,0);
		glVertex3f(i, 0, -nz);
		glVertex3f(i, 0, nz);
	}
	for(int i = -ny; i <= ny; i++){
		glColor3f(1,1,1);
		if(!i) glColor3f(1,0,0);
		glVertex3f(-nx, i, 0);
		glVertex3f(nx, i, 0);
		if(!i) glColor3f(0,1,0);
		glVertex3f(0, i, -nz);
		glVertex3f(0, i, nz);
	}
	for(int i = -nz; i <= nz; i++){
		glColor3f(1,1,1);
		if(!i) glColor3f(0,0,1);
		glVertex3f(0,-ny, i);
		glVertex3f(0,ny, i);
		if(!i) glColor3f(1,0,0);
		glVertex3f(-nx, 0, i);
		glVertex3f(nx, 0, i);

	}
	glEnd();
}

