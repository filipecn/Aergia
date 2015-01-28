/* 
 * GraphicsDisplay.cpp
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

#include "GraphicsDisplay.h"
using namespace aergia::io;

#include <iostream>
using namespace std;

GraphicsDisplay* GraphicsDisplay::instance = NULL;

GraphicsDisplay::GraphicsDisplay()
	: window(NULL),
	title(),
	width(400),
	height(400){
	}

GraphicsDisplay::GraphicsDisplay(int w, int h, const string& windowTitle){
	this->width = w;
	this->height = h;
	this->title = windowTitle;

	this->renderCallback = NULL;
	this->buttonCallback = NULL;
	this->keyCallback = NULL;
	this->mouseCallback = NULL;
	this->scrollCallback = NULL;
	this->keyCallback = NULL;
}

GraphicsDisplay::~GraphicsDisplay(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

GraphicsDisplay* GraphicsDisplay::create(int w, int h, const string& windowTitle, unsigned int flags){
	static GraphicsDisplay *_instance = new GraphicsDisplay(w,h,windowTitle);

	if(!instance)
		instance = _instance;
	instance->flags = flags;
	if(instance->init()) 
		return instance;
	return NULL;
}

bool GraphicsDisplay::init(){
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);

	if (!window){
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback (window, button_callback);
	glfwSetCursorPosCallback (window, pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewInit();

	GUIManager::getInstance();
	return true;
}

void GraphicsDisplay::setBackgroundColor(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
}

///////////////////////////////////////GUI/////////////////////////////////////////////////////
void GraphicsDisplay::add(GUI *g){
	GUIManager::getInstance().add(g);
}
///////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////SCENE/////////////////////////////////////////////////////
void GraphicsDisplay::setOrthographicProjection(double l, double r, double b, double t){
}

void GraphicsDisplay::add(SceneObject *so){
	SceneManager::getInstance().add(so);
}
///////////////////////////////////////////////////////////////////////////////////////////////

vec2 GraphicsDisplay::getMousePos(){
	double x, y;
	glfwGetCursorPos(this->window, &x, &y);	
	return vec2(x, this->height - y);
}

void GraphicsDisplay::getMousePos(double &x, double &y){
	glfwGetCursorPos(this->window, &x, &y);	
	y = this->height - y;
}

void GraphicsDisplay::start(){
    while(!glfwWindowShouldClose(this->window)){
        if(this->renderCallback) {
            this->renderCallback();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /*
    while(!glfwWindowShouldClose(this->window)){

		glfwGetFramebufferSize(window, &this->width, &this->height);
		glViewport(0, 0, this->width, this->height);

		glClear(GL_COLOR_BUFFER_BIT);

		if(this->renderCallback) {
			this->renderCallback();
		}
		glClearColor(0, 0, 0, 0);

		GUIManager::getInstance().resize(this->width,this->height);
		GUIManager::getInstance().render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	*/
}

void GraphicsDisplay::stop(){
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void GraphicsDisplay::error_callback(int error, const char* description){
	fputs(description, stderr);
}

void GraphicsDisplay::registerRenderFunc(void (*f)()){
	this->renderCallback = f;
}

/////////////////////////// KEY FUNCTIONS ////////////////////////////////////////////////////
void GraphicsDisplay::registerKeyFunc(void (*f)(int,int)){
	this->keyCallback = f;
}

void GraphicsDisplay::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(instance->keyCallback)
		instance->keyCallback(key,action);
	else instance->keyFunc(key,action);
}

void GraphicsDisplay::keyFunc(int key, int action){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// BUTTON FUNCTIONS //////////////////////////////////////////////////
void GraphicsDisplay::registerButtonFunc(void (*f)(int,int)){
	this->buttonCallback = f;
}

void GraphicsDisplay::button_callback(GLFWwindow* window, int button, int action, int mods){
	//button = 
	if(instance->buttonCallback)
		instance->buttonCallback(button,action);
	else instance->buttonFunc(button,action);
}

void GraphicsDisplay::buttonFunc(int button, int action){
	GUIManager::getInstance().processButton(button, action, getMousePos());
}
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// MOUSE MOTION FUNCTIONS ////////////////////////////////////////////
void GraphicsDisplay::registerMouseFunc(void (*f)(double,double)){
	this->mouseCallback = f;
}

void GraphicsDisplay::pos_callback(GLFWwindow* window, double x, double y){
	if(instance->mouseCallback)
		instance->mouseCallback(x,y);
	else instance->mouseFunc(x,y);
}

void GraphicsDisplay::mouseFunc(double x, double y){
	GUIManager::getInstance().processMouse(getMousePos());
}
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// MOUSE SCROLL FUNCTIONS ////////////////////////////////////////////
void GraphicsDisplay::registerScrollFunc(void (*f)(double,double)){
	this->scrollCallback = f;
}

void GraphicsDisplay::scroll_callback(GLFWwindow* window, double x, double y){
	if(instance->scrollCallback)
		instance->scrollCallback(x,y);
	else 
		instance->scrollFunc(x,y);
}

void GraphicsDisplay::scrollFunc(double x, double y){
	GUIManager::getInstance().processScroll(getMousePos(),vec2(x,y));
}

///////////////////////////////////////////////////////////////////////////////////////////////
