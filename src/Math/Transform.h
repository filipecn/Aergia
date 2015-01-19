/* 
 * Transform.h
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

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <MathUtils.h>
#include <glm/glm.hpp>
using glm::vec3;
using glm::mat4;
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
using glm::translate;
using glm::rotate;
using glm::scale;
//#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using glm::quat;
using glm::toMat4;

namespace aergia {

	namespace math {
		class Transform {
			public:
				Transform();

				Transform operator*(const Transform &t) const;
				Transform &operator*=(const Transform &t);

				vec3 operator*(const vec3 &p) const;

				Transform &setIdentity();
				Transform &setScale(const double s);
				Transform &setTranslate(const vec3 &v);

				void rotate(double angle, const vec3 axis); 
				vec3 getRotationAxis();
				Transform &setRotate(const quat &q);

				mat4 matrix() const;
				mat4 inverseMatrix() const;
				void fromMatrix(const mat4 &m);

				quat r;
				vec3 t;
				double s; 
		};
	}
}

#endif

