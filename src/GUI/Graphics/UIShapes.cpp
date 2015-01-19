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

#include "UIShapes.h"
using namespace aergia::gui::graphics;

#include <iostream>
using namespace std;

Rectangle::Rectangle(vec4 _bbox){
	this->bbox[0] = vec3(_bbox.x,_bbox.y,0.0);
	this->bbox[1] = vec3(_bbox.z,_bbox.w,0.0);
	pos.x = bbox[0].x;
	pos.y = bbox[0].y;
}

void Rectangle::matchTexture(string name){
	reset();

	texture = Texture(name);
	vec2 size = texture.getSize();
	vec4 uvBBox = texture.getBBox();

	cout << "POS " << pos << "SIZE " << size << " UV " << uvBBox << endl; 

	bbox[0] = vec3(screenToGUICoordinates(vec2(pos.x, pos.y + size.y)),0.0);
	bbox[1] = vec3((float)(size.x), (float)(size.y), 0.0f);
	
	// convert pos and size to norm space
	vec3 p = guiToNormDevCoord(vec2(pos.x,pos.y));
	vec3 s = glm::abs(p - guiToNormDevCoord(size));
	
	cout << "POS " << p << "SIZE " << s << endl; 
	dataSize = 2 + 2;
	n_vData = 4;
	vData = new GLfloat[n_vData*dataSize];
	vData[0] = p.x; vData[1] = p.y; 
	vData[2] = uvBBox.x; vData[3] = uvBBox.y;

	vData[4] = p.x + s.x; vData[5] = p.y; 
	vData[6] = uvBBox.z; vData[7] = uvBBox.y;

	vData[8] = p.x + s.x; vData[9] = p.y - s.y; 
	vData[10] = uvBBox.z; vData[11] = uvBBox.w;

	vData[12] = p.x; vData[13] = p.y - s.y; 
	vData[14] = uvBBox.x; vData[15] = uvBBox.w;

	n_indices = 6;
	indices = new GLuint[n_indices];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	init();

	shader.set("texture");
	shader.vDataSize = dataSize;
	shader.addVertexAttribute("position", 2, 0);
	shader.addVertexAttribute("texcoord", 2, 2);
	shader.setUniform("texKitten", 0);

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
