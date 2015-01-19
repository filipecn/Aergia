/* 
 * Shapes.h
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

#ifndef SHAPES_H
#define SHAPES_H

#include <OpenGL.h>

#include <SceneObject.h>
using aergia::graphics::scene::SceneObject;
#include <DisplayUtils.h>
using namespace aergia::io;

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;

namespace aergia {

	namespace graphics {

		namespace helpers {

			class Rectangle : public SceneObject {
				public:
					Rectangle(vec4 _bbox);

					void draw(unsigned int mode = 0xffffff);
				
				protected:

			};

			class Box : public SceneObject {
				public: 
					Box(vec3 center, vec3 size);

					void draw(unsigned int mode = 0xffffff);
					
					vec3 center;
					vec3 size;
			};
		}
	}
}

#endif
