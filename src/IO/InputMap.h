/* 
 * InputMap.h
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

#ifndef INPUTMAP_H
#define INPUTMAP_H

#include <OpenGL.h>
#include <GLFW/glfw3.h>

#include <Config.h>
using aergia::common::Config;

namespace aergia {

	namespace io {

#define A_RELEASE 	GLFW_RELEASE
#define A_PRESS 	GLFW_PRESS

#define A_RIGHT_BUTTON 	GLFW_MOUSE_BUTTON_RIGHT
#define A_LEFT_BUTTON 	GLFW_MOUSE_BUTTON_LEFT  
#define A_MIDDLE_BUTTON GLFW_MOUSE_BUTTON_MIDDLE   

		class InputMap {
			private:
				InputMap();
			public:
				inline static InputMap& getInstance(){
					static InputMap im;
					return im;
				}

				~InputMap(){};


		};

	}
}

#endif
