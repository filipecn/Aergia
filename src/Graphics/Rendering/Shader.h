/* 
 * Shader.h
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

#ifndef SHADER_H
#define SHADER_H

#include <OpenGL.h>
#include <ShaderManager.h>
using aergia::graphics::rendering::ShaderManager;

#include <glm/glm.hpp>
using glm::mat4;
using glm::mat3;
using glm::vec2;
using glm::vec3;
using glm::vec4;

#include <vector>
using namespace std;

namespace aergia
{
	namespace graphics
	{
		namespace rendering
		{
			class Shader {
				public:
					Shader();
					Shader(const char* name);

					bool set(const char* name);

					bool start();
					void stop();
					
					int vDataSize;
					void addVertexAttribute(const char* attribute, int size, int offset);
					// Uniforms
					void setUniform(const char* name, mat4 m);
					void setUniform(const char* name, mat3 m);
					void setUniform(const char* name, vec4 v);
					void setUniform(const char* name, vec3 v);
					void setUniform(const char* name, vec2 v);
					void setUniform(const char* name, int i);

				protected:
					char name[100];
					GLuint id;
					
					struct VertexAttribute{
						const char* name;
						int offset;
						int size;
					};
					
					vector<VertexAttribute> vertexAttributes;

					GLint getUniLoc(const GLchar *name);
			};
		}
	}
}

#endif
