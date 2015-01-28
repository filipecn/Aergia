/*
 * FluidSimulator.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 FilipeCN
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

#include "FluidSimulator.h"
using namespace aergia::algorithms;

#include <iostream>
using namespace std;

FluidSimulator::FluidSimulator(){
    curStep = 0;
}

FluidSimulator::~FluidSimulator(){

}

GLuint FluidSimulator::generateTexture(vec3 size){
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch ((int)size.z) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, (GLsizei) size.x, (GLsizei) size.y, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, (GLsizei) size.x, (GLsizei) size.y, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (GLsizei) size.x, (GLsizei) size.y, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei) size.x, (GLsizei) size.y, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }

    CHECK_GL_ERRORS;

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    CHECK_GL_ERRORS;
    CHECK_FRAMEBUFFER;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return texture;
}

void FluidSimulator::swap(){
    GLuint *tmp = dst;
    dst = src;
    src = tmp;
}

bool FluidSimulator::init(vec2 gridSize) {
    this->gridSize = gridSize;

    glGenVertexArrays(1, &junkVAO);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    src = new GLuint[1];
    dst = new GLuint[1];

    src[0] = generateTexture(vec3(gridSize.x,gridSize.y,1));
    dst[0] = generateTexture(vec3(gridSize.x,gridSize.y,1));

    advectShader.loadFiles("advect", "shaders");
    advectShader.setUniform("tex", 0);
    texShader.loadFiles("screenTexture", "shaders");
    texShader.setUniform("tex", 0);
    simpleShader.loadFiles("simple", "shaders");

    return true;
}

void FluidSimulator::step() {
    glViewport(0, 0, (GLsizei) gridSize.x, (GLsizei) gridSize.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst[0], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, src[0]);

    advectShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    advectShader.end();

    swap();
}

void FluidSimulator::render(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, src[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    texShader.end();
}

