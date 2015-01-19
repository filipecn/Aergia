/* 
 * InteractionMethod.h
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

#ifndef INTERACTIONMETHOD_H
#define INTERACTIONMETHOD_H

#include <InputMap.h>
using namespace aergia::io;
#include <glm/glm.hpp>
using glm::vec2;

namespace aergia {
	
	namespace gui {
		
		namespace ui {

			class InteractionMethod {
				public:
					InteractionMethod(){}
					virtual ~InteractionMethod();

					virtual void draw(unsigned int mode = 0){}
					
					// MOUSE INPUTS
					virtual void buttonRelease(int button, vec2 p){}
					virtual void buttonPress(int button, vec2 p){}
					virtual void mouseMove(vec2 p){}
					virtual void mouseScroll(vec2 p, vec2 d){}

				protected:
					bool dragging;
					bool buttonsStates[5];

					vec2 pressButtonPosition;
					vec2 curMousePosition;
			};
		}
	}
}

#endif
