/* 
 * GUIUtils.h
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

#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <OpenGL.h>
#include <MathUtils.h>
using namespace aergia::math;
#include <DisplayUtils.h>
using namespace aergia::io;
#include <Sphere.h>
using aergia::math::sphere;
#include <Plane.h>
using aergia::math::plane;
#include <Line.h>
using aergia::math::line;
#include <Trackball.h>
using aergia::gui::ui::Trackball;
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;

namespace aergia {

	namespace gui {

		namespace graphics {
			void drawCircle();
			void drawSphere(Trackball tb);

			bool hitHyper(Trackball tb, vec3 viewpoint, plane vp, vec3 hitplane, vec3 &hit);
			vec3 hitSphere(Trackball tb, const vec3 p);
			vec3 hitPlane(Trackball tb, const vec3 p);
		}
	}
}

#endif
