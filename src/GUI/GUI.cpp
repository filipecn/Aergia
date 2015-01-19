/* 
 * GUI.cpp
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

#include "GUI.h"

#include <iostream>
using std::cout;

using namespace aergia::gui;

GUI::GUI(){
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		curIMethod = 0;
		bbox = viewportMatrix();
		backgroundColor = vec4(0,0,0,0);
		iMethods.push_back(new TrackballInterface(SceneManager::getInstance().getCurCamera()));
	}

	mouseCallback = NULL;
	mouseMoveCallback = NULL;

	graphic = NULL;
	isAnchor = false;
	isPassive = false;
}

GUI::~GUI(){}

void GUI::setViewport(){
	glEnable(GL_SCISSOR_TEST);
	
	glViewport((GLint) bbox.x, (GLint) bbox.y, (GLsizei) bbox.z, (GLsizei) bbox.w);
	glScissor((GLint) bbox.x, (GLint) bbox.y, (GLsizei) bbox.z, (GLsizei) bbox.w);
	
	glClear(GL_COLOR_BUFFER_BIT);
}

void GUI::render(unsigned int mode){
	setViewport();
	if(graphic)
		graphic->render();
	if(curIMethod < iMethods.size())
		iMethods[curIMethod]->draw();
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		SceneManager::getInstance().draw(bbox);
	}
	for(auto g : guis)
		g->render();
}

void GUI::add(GUI* gui){
	guis.push_back(gui);
}

void GUI::resize(double w, double h){
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		bbox = viewportMatrix();
	}
	for(auto gui : guis)
		gui->resize(w,h);
}

//////////////////////////MOUSE INTERACTIONS//////////////////////////////////////////////////

bool GUI::processButton(int button, int action, vec2 p){
	if(!checkPointInsideAABB(bbox, p))
		return false;
	for(auto gui : guis){
		if(gui->processButton(button,action,p)){
			if(!gui->isPassive)
				return true;
			break;
		}
	}
	if(curIMethod >= iMethods.size())
		return false;
	if(action == A_PRESS)
		iMethods[curIMethod]->buttonPress(button, p);
	if(action == A_RELEASE)
		iMethods[curIMethod]->buttonRelease(button, p);
	return true;
}

bool GUI::processMouse(vec2 p){
	if(!checkPointInsideAABB(bbox, p))
		return false;
	for(auto gui : guis){
		if(gui->processMouse(p))
			break;
	}
	if(curIMethod >= iMethods.size())
		return false;
	iMethods[curIMethod]->mouseMove(p);
	return true;
}

bool GUI::processScroll(vec2 p, vec2 dp){
	if(!checkPointInsideAABB(bbox, p))
		return false;
	for(auto gui : guis){
		if(gui->processScroll(p,dp))
			break;
	}
	if(curIMethod >= iMethods.size())
		return false;
	iMethods[curIMethod]->mouseScroll(p,dp);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void GUI::registerMouseClickFunc(void (*f)(int)){
	mouseClickCallback = f;
}
