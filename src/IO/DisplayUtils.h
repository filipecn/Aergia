/* 
 * DisplayUtils.h
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

#ifndef DISPLAYUTILS_H
#define DISPLAYUTILS_H

#include <OpenGL.h>

#include <SceneManager.h>
using aergia::graphics::scene::SceneManager;
#include <MathUtils.h>
using namespace aergia::math;
#include <Line.h>
using aergia::math::line;
#include <Plane.h>
using aergia::math::plane;
#include <Plane.h>
#include <Plane.h>
using aergia::math::plane;
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec4;
using glm::mat4;
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
using glm::inverse;
using glm::transpose;
#include <glm/gtc/type_ptr.hpp>
using glm::make_mat4;

namespace aergia {

	namespace io {
		
		vec2 screenToGUICoordinates(vec2 p);
		vec3 guiToNormDevCoord(vec2 p);
		vec2 normalizedScreenCoordinates(vec2 p);
		vec3 windowCoordToNormDevCoord(vec3 p);
		vec4 viewportMatrix();
		mat4 modelviewMatrix(bool fromGL = false);
		mat4 projectionMatrix(bool fromGL = false);
		vec3 viewPoint();
		plane viewPlane(vec3 p);
		vec3 unProject(vec3 p);

		line viewLineFromWindow(vec3 p);

	}
}

#endif
