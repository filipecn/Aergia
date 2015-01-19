/* 
 * Camera.cpp
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

#include "Camera.h"

using namespace aergia::graphics::scene;

#include <iostream>
using std::cout;
using std::endl;
#include <math.h>

Camera::Camera()
	: zoom(1)
{
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		pos = vec3(0,0,0);
		r = vec2(1,1);
		zoom = 1;
		display = vec2(1,1);
	}
}

void Camera::resize(double w, double h){
	display = vec2(w,h);
	ratio = w/h;
	r.x = r.y = zoom;
	if(w < h)
		r.y = r.x/ratio;
	else r.x = r.y*ratio;
}

void Camera::look(){
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	gluOrtho2D(pos.x - r.x, pos.x + r.x, pos.y - r.y, pos.y + r.y);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void Camera::rotate(){
	
}

void Camera::zoomIn(){
	this->zoom *= 1.03;
	resize(display.x,display.y);
}

void Camera::zoomOut(){
	this->zoom *= 0.97;
	resize(display.x,display.y);
}

// Control Functions
void Camera::stop(){
}

void Camera::panStart(vec2 p){
}

void Camera::rotateStart(vec2 p){
}

void Camera::update(vec2 p){
} 


Camera3D::Camera3D(){
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		float halfH = 10.0;
		pos = vec3(0,0,halfH/tan(22.5*3.1415/180.0));
		pos = vec3(0,0,10);
		target = vec3(0,0,0);
		up = vec3(0,1,0);

		projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.f);
		view = glm::lookAt(pos,target,up);
		model = mat4(1.0f);
	}
}

void Camera3D::look(){
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	glMultMatrixf(&projection[0][0]);

	//glMatrixMode(GL_MODELVIEW);

//	glLoadIdentity();

	glMultMatrixf(&view[0][0]);
	
	glMultMatrixf(&model[0][0]);

	glMatrixMode(GL_MODELVIEW);
}

void Camera3D::apply(Transform t){
	//mat4 m = t.matrix();
	//vec3 axis = t.getRotationAxis();
	//cout << "apply!\n" << axis << " " << glm::angle(t.r) << endl;
	//vec3 camAxis = glm::normalize(glm::inverse(glm::mat3(view))*t.getRotationAxis());  
	//cout << "apply!\n" << camAxis << endl;
	//cout << pos << endl;
	//mat4 m = glm::toMat4(glm::angleAxis(glm::angle(t.r),camAxis));
	mat4 m = glm::toMat4(t.r);
	model = m;

	model = translate(m, t.t);
	//pos = m*pos;
		
	//view = glm::lookAt(pos,target,up);
}
