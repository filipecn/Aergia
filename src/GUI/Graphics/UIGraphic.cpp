/* 
 * UIGraphic.cpp
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

#include "UIGraphic.h"
using namespace aergia::gui::graphics;

#include <iostream>
using namespace std;

bool UIGraphic::checkMouseCollision(vec2 p){
	cout << p << endl;
	cout << bbox << endl;
	return checkPointInsideAABB(bbox, p);
}

void UIGraphic::set(unsigned int mode){
	switch(mode){
		case A_MOUSE_OUT : curAction = OUT; break;
		case A_MOUSE_OVER : curAction = (curAction == PRESSED)? PRESSED : OVER; break;
	}
}

void UIGraphic::set(int button, int action, vec2 p){
	curButton = button;
	curAction = action;
	switch(action){
		case A_PRESS : curAction = PRESSED; break;
		case A_RELEASE : curAction = OVER; break;
	}
	mousePos = p;
}

void UIGraphic::update(vec2 p){
	mousePos = p;
}

TextureGUI::TextureGUI(string name, vec2 p){
	cout << p.x << " " << p.y << endl;
	vec4 rectBBox(p.x,p.y,1,1);
	graphics = vector<Rectangle>(3,Rectangle(rectBBox));

	graphics[PRESSED].matchTexture(name + string("_PRESSED"));
	graphics[OUT].matchTexture(name + string("_OUT"));
	graphics[OVER].matchTexture(name + string("_OVER"));

	curAction = 1;
}

void TextureGUI::render(){
	graphics[curAction].draw();
}

bool TextureGUI::checkMouseCollision(vec2 p){
	// TODO make pixel collision instead!
	vec4 _bbox = vec4(
			graphics[curAction].bbox[0].x,
			graphics[curAction].bbox[0].y,
			graphics[curAction].bbox[1].x,
			graphics[curAction].bbox[1].y);
	return checkPointInsideAABB(_bbox, p);
}
