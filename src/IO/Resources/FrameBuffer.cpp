/* 
 * FrameBuffer.cpp
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

#include "FrameBuffer.h"
using namespace aergia::io::resources;

#include <iostream>
using namespace std;

FrameBuffer::FrameBuffer(){}

FrameBuffer::~FrameBuffer() {
	if(texColorBuffer)
		glDeleteTextures(1, &texColorBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(frameBuffer)
		glDeleteFramebuffers(1, &frameBuffer);
}

FrameBuffer::FrameBuffer(vec2 size){
	this->size = size;

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) size.x, (GLsizei) size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, texColorBuffer, 0);

	//renderBuffer = 0;
	//glGenRenderbuffers(1, &renderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
//			(GLsizei) size.x, (GLsizei) size.y);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
			GL_FRAMEBUFFER_COMPLETE)
		exit(1);
}

void FrameBuffer::enable(){
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0,0, (GLsizei) size.x, (GLsizei) size.y);
	// frame buffer attach
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//		GL_TEXTURE_2D, texColorBuffer, 0);
	//glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

}

void FrameBuffer::disable(){
	GLubyte pixels[4*4*4];
	glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


