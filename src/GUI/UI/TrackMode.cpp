/* 
 * TrackMode.cpp
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

#include "TrackMode.h"
using namespace aergia::gui::ui;
#include <math.h>
#include <iostream>
using namespace std;

TrackMode::~TrackMode(){}

void TrackMode::start(Trackball &tb, vec2 p){
	startPoint = vec3(p,0);
	oldPoint = vec3(p,0);
	dragging = true;
	cout << "starting! " << p << " " << oldPoint << std::endl;
	tb.pushTransform();
}

void TrackMode::stop(Trackball &tb){
	dragging = false;
}

/////////////////////////////// ROTATE MODE ////////////////////////////////////////////////
void RotateMode::draw(Trackball tb){
	drawSphere(tb);
	glBegin(GL_LINE);
	glVertex3f(axis.x,axis.y,axis.z);
	glVertex3f(tb.getCenter().x,tb.getCenter().y,tb.getCenter().z);
	glEnd();
}

void RotateMode::update(Trackball &tb, vec2 p){
	if(!dragging)
		return;
	vec3 oldP = hitSphere(tb, oldPoint);  
	vec3 newP = hitSphere(tb, vec3(p,0));
	cout << "oldp " << oldP << " newP " << newP << std::endl;
	vec3 center = tb.getCenter();
	axis = glm::normalize(glm::cross((oldP - center),(newP - center)));
	
	float phi = glm::distance(newP, oldP) / tb.radius;
	//cout << "-> " << tb.transform.r << " " << phi << " "  << axis << std::endl;
	// /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
	
	//phi = acos(std::min(1.0f, glm::dot(glm::normalize(newP - center),glm::normalize(oldP - center))));
					
	// /* Cross product to get the rotation axis, but it's still in camera coordinate */
	//a camAxis = glm::cross( prevPos, currPos );
	cout << axis << endl;
	vec3 newAxis = glm::normalize(glm::inverse(
				
				glm::mat3(
					glm::toMat4(tb.transform.r)
					*
					modelviewMatrix()
					))*axis);  
	cout << newAxis << endl;
	//tb.transform.r = glm::angleAxis(glm::degrees(phi)*0.4f, newAxis);
	tb.transform.r = glm::rotate(tb.transform.r, glm::degrees(phi)*0.8f, newAxis);
	oldPoint = vec3(p,0);
}

////////////////////////////// PAN MODE ///////////////////////////////////////////////////
void PanMode::draw(Trackball tb){
	
}

void PanMode::update(Trackball &tb, vec2 p){
	if(!dragging)
		return;

	vec3 oldP = hitPlane(tb, oldPoint);
	vec3 newP = hitPlane(tb, vec3(p,0));
	
	tb.transform.t += newP - oldP;
	
	oldPoint = vec3(p,0);

}

void ZMode::update(Trackball &tb, vec2 d){
	
}
