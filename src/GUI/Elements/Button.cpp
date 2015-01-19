/* 
 * Button.cpp
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

#include "Button.h"
using namespace aergia::gui;

#include <iostream>
#include <string>

Button::Button(string text, float x, float y){
	graphic = NULL;
	if(Config::status(A_DEFAULT)){
		graphic = new TextureGUI(string("defaultPushButton"),vec2(x,y));
	}

	mouseCallback = NULL;
	mouseMoveCallback = NULL;
}

void Button::render(unsigned int mode){
	graphic->render();
}

bool Button::processButton(int button, int action, vec2 p){
	bool mouseColliding = graphic->checkMouseCollision(p);
	if(!isAnchor && !mouseColliding)
		return false;
	if(mouseCallback)
		mouseCallback(button, action, p.x, p.y);
	if(mouseColliding)
		graphic->set(button,action,p);
	else graphic->set(A_MOUSE_OUT);

	if(action == A_PRESS)
		GUIManager::getInstance().setAnchor(this);
	if(action == A_RELEASE){
		GUIManager::getInstance().removeAnchor();
		if(mouseClickCallback)
			mouseClickCallback(button);
	}
	return true;
}

bool Button::processMouse(vec2 p){
	if(!graphic->checkMouseCollision(p)){
		if(!isAnchor)
			graphic->set(A_MOUSE_OUT);
		return false;
	}
	graphic->set(A_MOUSE_OVER);
	if(mouseCallback)
		mouseMoveCallback(p.x, p.y);
	graphic->update(p);
	return true;
}
