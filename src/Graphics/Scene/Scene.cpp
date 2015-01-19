/* 
 * Scene.cpp
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

#include "Scene.h"

using namespace aergia::graphics::scene;

Scene::Scene(){
	curCamera = 0;
	if(Config::getInstance().GENERAL_BEHAVIOUR == A_DEFAULT){
		cameras.push_back(new Camera3D());
	}
}

Scene::~Scene(){}

bool Scene::init(){
	return true;
}

void Scene::setDisplay(vec4 display){
	if(curCamera < cameras.size())
		cameras[curCamera]->resize(display.z,display.w);
}

void Scene::draw(unsigned int mode){
	if(curCamera < cameras.size())
		cameras[curCamera]->look();
	for(auto so : objects)
		so->draw();
}

void Scene::add(SceneObject *so){
	objects.push_back(so);
}

Camera* Scene::getCurCamera(){
	if(curCamera >= cameras.size())
		return NULL;
	return cameras[curCamera];
}
