/* 
 * TrackballInterface.cpp
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

#include "TrackballInterface.h"
using namespace aergia::gui::ui;

#include <iostream>
using std::cout;

TrackballInterface::TrackballInterface(SceneObject* so){
	curMode = 0;
	modes.push_back(new RotateMode());
	modes.push_back(new PanMode());
	
	this->so = so;
	
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
	}
}

TrackballInterface::~TrackballInterface(){

}

void TrackballInterface::draw(unsigned int mode){
	if(modes.size() <= curMode) 
		return;
	modes[curMode]->draw(tb);
}

void TrackballInterface::buttonRelease(int button, vec2 p){
	if(modes.size() <= curMode) 
		return;
	modes[curMode]->stop(tb);
}

void TrackballInterface::buttonPress(int button, vec2 p){
	if(modes.size() <= curMode) 
		return;
	if(button == A_LEFT_BUTTON)
		curMode = 0;
	if(button == A_RIGHT_BUTTON)
		curMode = 1;

	modes[curMode]->start(tb,p);
}

void TrackballInterface::mouseMove(vec2 p){
	if(modes.size() <= curMode) 
		return;
	modes[curMode]->update(tb,p);
	if(so)
		so->apply(tb.transform);
}

void TrackballInterface::mouseScroll(vec2 p, vec2 d){
	if(modes.size() <= curMode) 
		return;
	modes[curMode]->update(tb,d);
}
