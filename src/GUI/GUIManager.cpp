/* 
 * GUIManager.cpp
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

#include "GUIManager.h"

using namespace aergia::gui;

#include <iostream>

GUIManager::GUIManager(){
	curGUI = -1;
}

void GUIManager::add(GUI *gui){
	if(curGUI < 0){
		guis.push_back(new GUI());
		curGUI = 0;
	}

	guis[curGUI]->add(gui);
}

void GUIManager::setAnchor(GUI* a){
	anchor = a;
	anchor->isAnchor = true;
}

void GUIManager::removeAnchor(){
	if(anchor)
		anchor->isAnchor = false;
	anchor = NULL;
}

void GUIManager::render(unsigned int mode){
	for(auto gui : guis)
		gui->render();
}

void GUIManager::resize(double w, double h){

}

void GUIManager::processButton(int button, int action, vec2 p){
	if(anchor){
		anchor->processButton(button, action, p);
		return;
	}
	for(auto gui : guis)
		gui->processButton(button,action,p);
}

void GUIManager::processMouse(vec2 p){
	if(anchor){
		std::cout << "anchor exists!\n";
		anchor->processMouse(p);
		return;
	}
	std::cout << "anchor DONT exists!\n";
	for(auto gui : guis)
		gui->processMouse(p);
}

void GUIManager::processScroll(vec2 p, vec2 dp){
	if(anchor){
		anchor->processScroll(p, dp);
		return;
	}
	for(auto gui : guis)
		gui->processScroll(p,dp);
}
