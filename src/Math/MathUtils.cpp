/* 
 * MathUtils.h
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

#include "MathUtils.h"

#include <iostream>

namespace aergia {

	namespace math {

		bool checkPointInsideAABB(vec4 box, vec2 p){
			bool check = ((p.x >= box.x)&&(p.x <= (box.x + box.z))&&(p.y >= box.y)&&(p.y <= (box.y + box.w))); 
			return check; 
		}

		bool planeLineIntersection(plane pl, line ln, vec3 &hp){
			const double epsilon = 1e-8;
			float k = glm::dot(pl.normal,ln.direction());
		        if( (k > -epsilon) && (k < epsilon))
		             return false;
			float r = (pl.offset - glm::dot(pl.normal,ln.a))/k;
			hp = ln.a + ln.direction()*r;
			return true;
		}

		bool lineSphereIntersection (sphere s, line l, vec3 &h0, vec3 &h1){
			float a = glm::dot(l.direction(),l.direction());
			float b = 2.0*glm::dot(l.direction(), l.a - s.center);
			float c = glm::dot(l.a - s.center, l.a - s.center) - s.radius*s.radius;

			float delta = b*b - 4*a*c;
			if(delta < 0)
				return false;

			float d = sqrt(delta);

			h0 = l.point((-b+d)/(2.0*a));
			h1 = l.point((-b-d)/(2.0*a));
			return true;
		}
		
		double distance(vec3 p, plane pl){
			return 0.0;
		}

		ostream& operator<<(ostream &out, vec2 &v){
			out << "(" << v.x << "," << v.y  << ")";
			return out;
		}

		ostream& operator<<(ostream &out, vec3 &v){
			out << "(" << v.x << "," << v.y << "," << v.z << ")";
			return out;
		}

		ostream& operator<<(ostream &out, vec4 &v){
			out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
			return out;
		}

		ostream& operator<<(ostream &out, quat &q){
			out << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")";
			return out;
		}

		ostream& operator<<(ostream &out, mat4 &v){
			out << "(";
			for(int i = 0; i < 4; i++){
				out << v[0][i];
				for(int j = 1; j < 4; j++)
					out << "," << v[j][i];
				if(i < 3) out << endl;
				else out << ")\n";
			}
			return out;
		}

		vec3 operator*(const mat4& m, const vec3& p){
			vec4 P(p,1);
			vec4 s = m*P;
			if(s.w != 0){
				s.x /= s.w;
				s.y /= s.w;
				s.z /= s.w;
				s.w = 1.0;
			}
			return vec3(s.x,s.y,s.z);
		}
	}

}

