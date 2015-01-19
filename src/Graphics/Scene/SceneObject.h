/* 
 * SceneObject.h
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

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <OpenGL.h>

#include <Transform.h>
using aergia::math::Transform;
#include <Texture.h>
using aergia::io::resources::Texture;
#include <iostream>
using std::cout;
#include <Shader.h>
using aergia::graphics::rendering::Shader;

namespace aergia
{
	namespace graphics
	{
		namespace scene
		{
			class SceneObject {
				public:
					SceneObject();
					virtual ~SceneObject();
					
					virtual bool init();
					virtual void reset();

					virtual void draw(unsigned int mode = 0xffffff);
					virtual void apply(Transform t);
					
					GLfloat *vData;
					GLuint n_vData;
					GLuint dataSize;
					GLuint *indices;
					GLuint n_indices;
					GLenum elementType;

					Shader shader;
					Texture texture;
					vec3 pos;
					Transform transform;
				
					vec3 bbox[2];
					vec3 color;

				protected:
					GLuint vbo;
					GLuint ind;	
			};
		}
	}
}

#endif
