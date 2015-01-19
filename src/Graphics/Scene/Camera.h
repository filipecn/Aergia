/* 
 * Camera.h
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

#ifndef CAMERA_H
#define CAMERA_H

#include <OpenGL.h>
#include <Config.h>
using namespace aergia::common;
#include <SceneObject.h>
using aergia::graphics::scene::SceneObject;
#include <MathUtils.h>
using namespace aergia::math;
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::mat4;
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
using glm::perspective;
using glm::translate;
using glm::scale;

namespace aergia
{
	namespace graphics
	{
		namespace scene {
			class Camera : public SceneObject {
				public:
					Camera();

					bool is3D, is2D;

					virtual void resize(double w, double h);
					virtual void look();
					virtual void rotate();
					
					// Matrices
					inline mat4 getProjection(){
						return projection;
					}

					inline mat4 getView(){
						return view;
					}

					// Control Functions
					virtual void stop();
					virtual void panStart(vec2 p);
					virtual void rotateStart(vec2 p);
					virtual void update(vec2 p);
					virtual void zoomIn();
					virtual void zoomOut();

					vec3 pos;

				protected:
					double ratio;
					double zoom;

					vec2 r;
					vec2 display;
					
					mat4 projection;
					mat4 view;
					mat4 model;
			};

			class Camera3D : public Camera {
				public:
					Camera3D();

					void look();
					void apply(Transform t);

					vec3 target;
					vec3 up;
			};
		}
	}
}

#endif
