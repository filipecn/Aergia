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
    SRC = 0;
    DST = 1;
}

FluidSimulator::~FluidSimulator(){

}

void FluidSimulator::generateGrid(int G, vec2 size, GLint internalFormat, GLenum format,
        GLenum type){

    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &grids[G].t[i]);
        glBindTexture(GL_TEXTURE_2D, grids[G].t[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

/*
    switch ((int)size.z) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, (GLsizei) size.x, (GLsizei) size.y, 0, GL_RED, GL_FLOAT, 0);
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
*/
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) size.x, (GLsizei) size.y, 0,
                format, type, 0);

        CHECK_GL_ERRORS;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, grids[G].t[i], 0);

        CHECK_GL_ERRORS;
        CHECK_FRAMEBUFFER;

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    grids[G].size = size;
    grids[G].internalFormat = internalFormat;
    grids[G].format = format;
    grids[G].type = type;
}

void FluidSimulator::swap(){
    int tmp = SRC;
    SRC = DST;
    DST = tmp;
}

void FluidSimulator::printGrid(GLuint grid, vec2 size){
    float * data = new float[(int)(size.x*size.y)];
    for(int i(0); i < (int)size.y; ++i) {
        for(int j(0); j < (int)size.x; ++j) {
            data[i * (int)size.x + j] = 0.0f;
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grid);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);

    CHECK_GL_ERRORS;

    cout << "printing grid:\n";
    for(int i(0); i < (int)size.y; ++i) {
        for(int j(0); j < (int)size.x; ++j) {
            cout << data[i * (int)size.x + j] << ",";
        }
        cout << endl;
    }
}

bool FluidSimulator::init(vec2 gridSize, float dx, float dt) {

    glGenVertexArrays(1, &junkVAO);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    this->gridSize = gridSize;
    this->cellSize = dx;
    this->timeStep = dt;

    generateGrid(U, vec2(gridSize.x+1,gridSize.y), GL_R16F, GL_RED, GL_FLOAT);
    grids[U].offset = vec2(-cellSize/2.0,0.0);
    grids[U].m = 0;

    generateGrid(V, vec2(gridSize.x,gridSize.y+1), GL_R16F, GL_RED, GL_FLOAT);
    grids[V].offset = vec2(-cellSize/2.0,0.0);
    grids[V].m = 1;

    generateGrid(Q, vec2(gridSize.x,gridSize.y), GL_R16F, GL_RED, GL_FLOAT);
    grids[Q].offset = vec2(-cellSize/2.0,0.0);
    grids[Q].m = 2;

    advectShader.loadFiles("advect", "shaders");

    advectShader.setUniform("u.m", 0);
    advectShader.setUniform("u.offset", grids[U].offset);

    advectShader.setUniform("v.m", 1);
    advectShader.setUniform("v.offset", grids[V].offset);

    advectShader.setUniform("q.m", 2);
    advectShader.setUniform("q.offset", grids[Q].offset);

    advectShader.setUniform("dt", timeStep);
    advectShader.setUniform("dx", cellSize);

    texShader.loadFiles("screenTexture", "shaders");
    texShader.setUniform("tex", 0);

    simpleShader.loadFiles("simple", "shaders");

    return true;
}

void FluidSimulator::step() {
    //return;
    glViewport(0, 0, (GLsizei) gridSize.x, (GLsizei) gridSize.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[Q].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[U].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[V].t[SRC]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[Q].t[SRC]);

    advectShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    advectShader.end();

    swap();

    curStep++;
}

void FluidSimulator::render(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[Q].t[SRC]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    texShader.end();
}

void FluidSimulator::setGrid(int i, const GLvoid *data) {
    int width = (int) grids[i].size.x;
    int height = (int) grids[i].size.y;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, grids[i].internalFormat, width, height, 0,
            grids[i].format, grids[i].type, data);

    CHECK_GL_ERRORS;

    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[i].t[SRC], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    texShader.begin();
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    texShader.end();

    CHECK_GL_ERRORS;
}
