/* 
 * SceneManager.h
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

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
using std::vector;

#include <Config.h>
using namespace aergia::common;
#include <Scene.h>
using aergia::graphics::scene::Scene;

namespace aergia {

	namespace graphics {

		namespace scene {

			class SceneManager {
				public:
					inline static SceneManager& getInstance(){
						static SceneManager sm;
						return sm;
					}
					
					~SceneManager();
					
					void draw(vec4 display, unsigned int mode = 0);
					void add(SceneObject* so);

					Camera* getCurCamera();
				protected:
					SceneManager();

					static SceneManager* instance;
					
					unsigned int curScene;
					vector<Scene> scenes;
			};
		}
	}
}

#endif


