/* 
 * DisplayUtils.cpp
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

#include "DisplayUtils.h"

#include <iostream>
using namespace std;

namespace aergia {

	namespace io {

		vec2 screenToGUICoordinates(vec2 p){
			vec4 v = viewportMatrix();
			return vec2(p.x, v.w - p.y);
		}

		vec3 guiToNormDevCoord(vec2 p){
			vec4 v = viewportMatrix();
			return vec3((p.x - v.x)/(v.z/2.0) - 1.0,
					1.0 - (p.y - v.y)/(v.w/2.0),
					0.0);
		}

		vec2 normalizedScreenCoordinates(vec2 p){
			vec4 v = viewportMatrix();
			return vec2(p.x/v.z,p.y/v.w);
		}

		vec3 windowCoordToNormDevCoord(vec3 p){
			vec4 v = viewportMatrix();  
			return vec3((p.x - v.x)/(v.z/2.0) - 1.0,
					(p.y - v.y)/(v.w/2.0) - 1.0,
					2*p.z - 1.0);
		}

		vec4 viewportMatrix(){
			GLint m[4];
			glGetIntegerv( GL_VIEWPORT, m );
			return vec4(m[0],m[1],m[2],m[3]);
		}

		mat4 modelviewMatrix(bool fromGL){
			if(fromGL){
				GLfloat m[16];
				glGetFloatv( GL_MODELVIEW_MATRIX, m);
				return make_mat4(m);
			}
			return SceneManager::getInstance().getCurCamera()->getView();
		}

		mat4 projectionMatrix(bool fromGL){
			if(fromGL){
				GLfloat m[16];
				glGetFloatv( GL_PROJECTION_MATRIX, m); 
				return make_mat4(m);
			}
			return SceneManager::getInstance().getCurCamera()->getProjection();
		}

		vec3 viewPoint(){
			//cout << "VIEW POINT\n";
			//mat4 P = modelviewMatrix();
			//cout << "MODEL VIEW:\n" << P << endl;
			//mat4 i = inverse(modelviewMatrix());
			//cout << "INVERSE MODEL VIEW:\n" << i << endl;
			return inverse(modelviewMatrix())*vec3(0,0,0);
		}

		vec3 unProject(vec3 p){
			//vec3 t = windowCoordToNormDevCoord(p);
			//cout << "window coord " << p << " " << t << endl;
			//mat4 M = projectionMatrix();
			//mat4 P = modelviewMatrix();
			//cout << "MODEL VIEW:\n" << P << endl;
			//cout << "PROJ MATRIX\n" << M;
			//mat4 MM = projectionMatrix()*modelviewMatrix();
			//cout << "PROJ MODEL MATRIX\n" << MM;
			//vec4 test = inverse(projectionMatrix()*modelviewMatrix())*vec4(windowCoordToNormDevCoord(p),1.0);
			//cout << "test " << test << endl;
			return inverse(projectionMatrix()*modelviewMatrix())*windowCoordToNormDevCoord(p);
		}
		
		line viewLineFromWindow(vec3 p){
			vec3 vp = viewPoint();
			//cout << "viewPoint " << vp << endl;
			vec3 pp = unProject(p);
			//cout << "unproject " << pp << endl;
			return line(vp,(pp-vp));
		}
		
		plane viewPlane(vec3 p){
			vec3 vp = viewPoint();
			vec3 n = vp - p;
			if(fabs(glm::length(n)) < 1e-8)
				n = vec3(0,0,0);
			else n = glm::normalize(n);
			return plane(n, glm::dot(n, p));
		}
	}
}
