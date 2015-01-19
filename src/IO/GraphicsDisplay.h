/* 
 * GraphicsDisplay.h
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

#pragma once

#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include <OpenGL.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
using std::string;

#include <Constants.h>
using namespace aergia::common;

#include <GUIManager.h>
using aergia::gui::GUIManager;
#include <GUI.h>
using aergia::gui::GUI;

#include <SceneManager.h>
using aergia::graphics::scene::SceneManager;

namespace aergia {

	namespace io {

		class GraphicsDisplay {
			public:
				~GraphicsDisplay();

				static GraphicsDisplay* create(int w, int h, const string& windowTitle, unsigned int flags = 0);

				// Graphics
				void setBackgroundColor(float r, float g, float b, float a = 0);
				void setOrthographicProjection(double l, double r, double b, double t);

				vec2 getMousePos();
				void getMousePos(double &x, double &y);
				
				// GUI
				void add(GUI* g);

				// Scene
				void add(SceneObject *so);

				// Run
				void start();
				void stop();

				// IO
				void registerRenderFunc(void (*f)());
				void registerButtonFunc(void (*f)(int,int));
				void registerKeyFunc(void (*f)(int,int));
				void registerMouseFunc(void (*f)(double,double));
				void registerScrollFunc(void (*f)(double,double));
			protected:
				GraphicsDisplay();
				GraphicsDisplay(int w, int h, const string& windowTitle);

				static GraphicsDisplay* instance;
				
				unsigned int flags;

				// WINDOW
				GLFWwindow* window;
				string title;
				int width;
				int height;
				
				bool init();

				// USER CALLBACKS
				void (*renderCallback)();
				void (*buttonCallback)(int,int);
				void (*keyCallback)(int,int);
				void (*mouseCallback)(double,double);
				void (*scrollCallback)(double,double);

				// DEFAULT CALLBACKS
				void buttonFunc(int button, int action);
				void keyFunc(int key, int action);
				void mouseFunc(double x, double y);
				void scrollFunc(double x, double y);

				// CALLBACKS
				static void error_callback(int error, const char* description);
				static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
				static void button_callback(GLFWwindow* window, int button, int action, int mods);
				static void pos_callback(GLFWwindow* window, double x, double y);
				static void scroll_callback(GLFWwindow* window, double x, double y);
		};
	}
}
#endif
