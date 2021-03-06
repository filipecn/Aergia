/* 
 * FrameBuffer.h
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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include<OpenGL.h>

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

namespace aergia {

	namespace io {

		namespace resources {
			
			class FrameBuffer {
				public:
					FrameBuffer();
					FrameBuffer(vec2 size);
					FrameBuffer(vec3 size);
					~FrameBuffer();

					void enable();
					void disable();
					
					vec3 size;

					GLuint frameBuffer;
					GLuint texColorBuffer;
					GLuint renderBuffer;
			};
		}
	}
}

#endif
