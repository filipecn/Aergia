/* 
 * Transform.cpp
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

#include "Transform.h"
using namespace aergia::math;

#include <iostream>
using namespace std;

Transform::Transform(){
	s = 1.0;
	t = vec3(0);
	r = quat(1,0,0,0);
}

mat4 Transform::matrix() const{
	return toMat4(r)*scale(vec3(s))*translate(t);
}

mat4 Transform::inverseMatrix() const {
	return glm::inverse(matrix());		
}

void Transform::rotate(const double angle, const vec3 axis){
	vec3 a = axis;
	cout << r << " " << angle << " ----->>> " << a << endl;
	//r = glm::rotate(r, glm::degrees(angle), axis);
	cout << r << endl;
}

vec3 Transform::getRotationAxis(){
	return glm::axis(r);
}
