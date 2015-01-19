/* 
 * TrackMode.h
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

#ifndef TRACKMODE_H
#define TRACKMODE_H

#include <DisplayUtils.h>
using namespace aergia::io;
#include <Trackball.h>
using aergia::gui::ui::Trackball;
#include <GUIUtils.h>
using namespace aergia::gui::graphics;

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

namespace aergia {
	
	namespace gui {
		
		namespace ui {
			
			class TrackMode {
				public: 
					virtual ~TrackMode();

					virtual void draw(Trackball tb){}

					// CONTROL
					virtual void start(Trackball &tb, vec2 p);
					virtual void update(Trackball &tb, vec2 p){}
					virtual void stop(Trackball &tb);

				protected:
					vec3 startPoint;
					vec3 oldPoint;
					
					bool dragging;
			};

			class RotateMode : public TrackMode {
				public:
					~RotateMode(){}
					
					void draw(Trackball tb);

					void update(Trackball &tb, vec2 p);
				
					vec3 axis;
			};

			class PanMode : public TrackMode {
				public:
					~PanMode(){}

					void draw(Trackball tb);

					void update(Trackball &tb, vec2 p);
			};

			class ZMode : public TrackMode {
				public:
					~ZMode(){}

					void update(Trackball &tb, vec2 d);
			};
		}
	}
}

#endif
