/* 
 * Constants.h
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace aergia {

	namespace common {

#define A_DEFAULT 	(1<<0)

///////////////////// GUI ////////////////////////////////////////////////////

#define A_TRACKBALL (1<<0)

#define A_FULL_DISPLAY 	(1<<0)
#define A_FULL_AREA 	(1<<1)
#define A_FULL_SCREEN 	(1<<2)
		
#define A_SCREEN    	(1<<2)
#define A_WORLD     	(1<<3)
#define A_CENTER 	(1<<4)

#define A_MOUSE_OUT     (1<<0)
#define A_MOUSE_OVER    (1<<1)
	}
}

#endif
