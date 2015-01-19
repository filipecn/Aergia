/* 
 * GUIUtils.cpp
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

#include "GUIUtils.h"

#include <iostream>
using std::cout;

namespace aergia {

	namespace gui {

		namespace graphics {
			void drawCircle(){
				int nside = 100;
				const double pi2 = 3.14159265 * 2.0;
				glBegin (GL_LINE_LOOP);
				for (double i = 0; i < nside; i++) {
					glNormal3d (cos (i * pi2 / nside), sin (i * pi2 / nside), 0.0);
					glVertex3d (cos (i * pi2 / nside), sin (i * pi2 / nside), 0.0);
				}
				glEnd ();
			}

			void drawSphere(Trackball tb){
				
				glPushAttrib(GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix ();
				glDepthMask(GL_FALSE);

				vec4 center = vec4(tb.getCenter(),0) + tb.transform.inverseMatrix()*vec4(0);
				glTranslatef(center.x,center.y,center.z);
				glScalef (
						(GLfloat) (tb.radius/tb.transform.s),
						(GLfloat) (tb.radius/tb.transform.s),
						(GLfloat) (tb.radius/tb.transform.s)
						);
				
				float amb[4] = { .35f, .35f, .35f, 1.0f };
				float col[4] = { .5f, .5f, .8f, 1.0f };
				glEnable (GL_LINE_SMOOTH);

				glDisable(GL_COLOR_MATERIAL); // has to be disabled, it is used by wrapper to draw meshes, and prevent direct material setting, used here

				glEnable (GL_LIGHTING);
				glEnable (GL_LIGHT0);
				glEnable (GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glColor (DH.color);
				glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, amb);

				col[0] = .40f; col[1] = .40f; col[2] = .85f;
				glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, col);
				drawCircle();

				glRotatef (90, 1, 0, 0);
				col[0] = .40f; col[1] = .85f; col[2] = .40f;
				glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, col);
				drawCircle();

				glRotatef (90, 0, 1, 0);
				col[0] = .85f; col[1] = .40f; col[2] = .40f;
				glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, col);
				drawCircle();

				glPopMatrix ();
				glPopAttrib ();
			}
			
			bool hitHyper (Trackball tb, vec3 viewpoint, plane vp, vec3 hitplane, vec3 &hit){
				float hitplaney = glm::distance(tb.getCenter(), hitplane);
				float viewpointx = glm::distance(tb.getCenter(), viewpoint);
				
				float a = hitplaney / viewpointx;
				float b = -hitplaney;
				float c = (float) ((tb.radius * tb.radius) / 2.0f);
				float delta = b * b - 4 * a * c;
				float x1, x2, xval, yval;

				if (delta > 0) {
					x1 = (-b - sqrt (delta)) / (2.0f * a);
					x2 = (-b + sqrt (delta)) / (2.0f * a);

					xval = x1;                   
					yval = c / xval;              
				}
				else 
					return false;
				vec3 dirRadial = glm::normalize(hitplane - tb.getCenter());
				vec3 dirView = glm::normalize(vp.normal);
				hit = tb.getCenter() + dirRadial * yval + dirView * xval;
				return true;
			}

			vec3 hitSphere(Trackball tb, const vec3 p){
				//cout << "pos " << p << endl;
				vec3 center = tb.getCenter();
				//cout << "center " << center << endl;
				line l = viewLineFromWindow(p);  
				//vec3 tl = l.direction();
				//cout << "line d " << tl << endl;
				plane vp = viewPlane(center);
				//cout << vp.normal << std::endl;
				vec3 hitPl(0,0,0), hitHyp(0,0,0);
				vec3 hitSphere(0,0,0), hitSphere1(0,0,0), hitSphere2(0,0,0);
				sphere s(center,tb.radius);

				planeLineIntersection(vp, l, hitPl);
				//cout << "HIT PLANE " << hitPl << endl;
				vec3 viewpoint = viewPoint();
				cout << "viewpoint " << viewpoint << endl;
				bool  resSp = lineSphereIntersection(s, l, hitSphere1, hitSphere2);
				//cout << "hitS 1 " << hitSphere1 << endl;
				//cout << "hitS 2 " << hitSphere2 << endl;
				//hitSphere.x *= -1;
				//hitSphere.y *= -1;
				//cout << "resP " << resSp << endl;
				if (resSp){
					if (glm::distance(viewpoint, hitSphere1) < glm::distance(viewpoint, hitSphere2))
						hitSphere = hitSphere1;
					else
						hitSphere = hitSphere2;
					return hitSphere;
				}
				cout << hitSphere << endl;
				
				bool resHp = hitHyper (tb, viewpoint, vp, hitPl, hitHyp);
				cout << resHp << endl;
				if ((!resSp && !resHp)) {
					vec3 hit = l.closestPoint(tb.getCenter());
					return hit;
				}

				if ((resSp && !resHp))
					return hitSphere;           
				if ((!resSp && resHp))
					return hitHyp;            
				float angleDeg = glm::degrees(
					asin(glm::dot(
								glm::normalize(viewpoint - tb.getCenter()), 
								glm::normalize(hitSphere - tb.getCenter()))));
				if (angleDeg < 45)
					return hitSphere;
				else
					return hitHyp;
			}

			vec3 hitPlane(Trackball tb, const vec3 p){
				line l = viewLineFromWindow(p);  
				plane vp = viewPlane(tb.getCenter());
				
				vec3 hit(0,0,0);
				planeLineIntersection(vp, l, hit);
				return hit;
			}
		}
	}
}
