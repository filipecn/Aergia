/* 
 * GUIManager.h
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

#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <vector>
using std::vector;
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec4;

#include <OpenGL.h>
#include <Config.h>
using namespace aergia::common;

#include <GUI.h>

namespace aergia {
	
	namespace gui {
	
		class GUIManager {
			public:
				static GUIManager& getInstance(){
					static GUIManager gm;
					return gm;
				}

				~GUIManager(){}
				
				void render(unsigned int mode = 0);

				// GUIs
				void add(GUI* gui);

				void resize(double w, double h);
				
				// MOUSE INTERACTIONS
				void  processButton(int button, int action, vec2 p);
				void  processMouse(vec2 p);
				void  processScroll(vec2 p, vec2 dp);

				void setAnchor(GUI* a);
				void removeAnchor();

			protected:
				GUIManager();
				
				struct guiInfo{
					string name;
					int id;
					int z;
				};

				// GUIs
				int curGUI;
				vector<GUI*> guis;

				GUI* anchor;
		};
	}
}

#endif
