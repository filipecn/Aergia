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
    density = 0.1;
    SRC = 0;
    DST = 1;

    rs = 1;
    rt = 1;
    ks = 1;
    kt = 1;
    Tamb = 273;

    bouyance_alpha = 1;
    bouyance_beta = 1;

    gravity = vec2(0.0,-10.0);
}

FluidSimulator::~FluidSimulator(){

}

void FluidSimulator::generateGrid(int G, vec2 size, GLint internalFormat, GLenum format,
        GLenum type, float *borderColor = NULL){

    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &grids[G].t[i]);
        glBindTexture(GL_TEXTURE_2D, grids[G].t[i]);
        if(borderColor == NULL) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }
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

void FluidSimulator::swap(int g) {
    GLuint tmp = grids[g].t[0];
    grids[g].t[0] = grids[g].t[1];
    grids[g].t[1] = tmp;
}

void FluidSimulator::printGrid(GLuint grid, vec2 size){
    float * data = new float[(int)(size.x*size.y)];
    for(int i((int)size.y-1); i >= 0; --i) {
        for(int j(0); j < (int)size.x; ++j) {
            data[i * (int)size.x + j] = 0.0f;
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grid);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);

    CHECK_GL_ERRORS;

    cout << "printing grid:\n";
    for(int i((int)size.y-1); i >= 0; --i) {
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

    generateGrid(U, vec2(gridSize.x+1,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);
    grids[U].offset = vec2(-0.5,0.0);
    grids[U].m = 0;

    generateGrid(V, vec2(gridSize.x,gridSize.y+1), GL_R32F, GL_RED, GL_FLOAT);
    grids[V].offset = vec2(0.0,-0.5);
    grids[V].m = 1;

    generateGrid(Q, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);
    grids[Q].offset = vec2(0.0);
    grids[Q].m = 2;

    float smokeBorder[] = {0,0,0,0};
    generateGrid(S, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT, smokeBorder);
    grids[S].offset = vec2(0.0);
    grids[S].m = 2;

    float tBorder[] = {273,273,273,273};
    generateGrid(T, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);//, tBorder);
    grids[T].offset = vec2(0.0);
    grids[T].m = 2;

    generateGrid(H, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);
    grids[H].offset = vec2(0.0);
    grids[H].m = 2;

    generateGrid(D, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);
    grids[D].offset = vec2(0.0);
    grids[D].m = 3;

    generateGrid(P, vec2(gridSize.x,gridSize.y), GL_R32F, GL_RED, GL_FLOAT);
    grids[P].offset = vec2(0.0);
    grids[P].m = 3;

    advectShader.loadFiles("advect", "shaders");
    advectShader.setUniform("u.m", 0);
    advectShader.setUniform("u.offset", grids[U].offset);
    advectShader.setUniform("u.size", grids[U].size);
    advectShader.setUniform("v.m", 1);
    advectShader.setUniform("v.offset", grids[V].offset);
    advectShader.setUniform("v.size", grids[V].size);
    advectShader.setUniform("q.m", 2);
    advectShader.setUniform("dt", timeStep);
    advectShader.setUniform("dx", cellSize);

    heatShader.loadFiles("heat", "shaders");
    heatShader.setUniform("t", 0);
    heatShader.setUniform("h", 1);
    heatShader.setUniform("s", (float)(1.0 - exp(-rt*timeStep)));

    smokeShader.loadFiles("smoke", "shaders");
    smokeShader.setUniform("d", 0);
    smokeShader.setUniform("s", 1);
    smokeShader.setUniform("c", rs*timeStep);

    forcesShader.loadFiles("forces", "shaders");
    forcesShader.setUniform("q.m", 0);
    forcesShader.setUniform("q.size", grids[V].size);
    forcesShader.setUniform("q.offset", grids[V].offset);
    forcesShader.setUniform("t.m", 1);
    forcesShader.setUniform("t.size", grids[T].size);
    forcesShader.setUniform("s.m", 2);
    forcesShader.setUniform("s.size", grids[Q].size);
    forcesShader.setUniform("alpha", bouyance_alpha);
    forcesShader.setUniform("beta", bouyance_beta);
    forcesShader.setUniform("Tamb", Tamb);
    forcesShader.setUniform("dt", timeStep);

    divergenceShader.loadFiles("divergence", "shaders");
    divergenceShader.setUniform("u.m", 0);
    divergenceShader.setUniform("u.size", grids[U].size);
    divergenceShader.setUniform("v.m", 1);
    divergenceShader.setUniform("v.size", grids[V].size);
    divergenceShader.setUniform("s", density/(cellSize*timeStep));

    jacobiShader.loadFiles("jacobi", "shaders");
    jacobiShader.setUniform("x.m", 0);
    jacobiShader.setUniform("x.size", grids[P].size);
    jacobiShader.setUniform("b.m", 1);
    jacobiShader.setUniform("b.size", grids[D].size);
    jacobiShader.setUniform("alpha", -cellSize*cellSize);
    jacobiShader.setUniform("beta", (float)(1.0/4.0));

    gradientShader.loadFiles("gradient", "shaders");
    gradientShader.setUniform("p.m", 0);
    gradientShader.setUniform("p.size", grids[P].size);
    gradientShader.setUniform("s", timeStep/(density*cellSize));

    texShader.loadFiles("screenTexture", "shaders");
    texShader.setUniform("tex", 0);

    simpleShader.loadFiles("simple", "shaders");

    return true;
}

void FluidSimulator::step() {
    static int k = 0;
    //if(k++ > 0)
      //  return;

    advect(V); swap(V);

    advect(U); swap(U);
    //printGrid(grids[U].t[SRC], grids[U].size);

    cout << "advect Q------------------------------------------------\n";
    //printGrid(grids[Q].t[SRC], grids[Q].size);
    advect(Q); swap(Q);
    printGrid(grids[Q].t[SRC], grids[Q].size);

    //cout << "addHEAT!\n";
    //printGrid(grids[T].t[SRC], grids[T].size);
    addHeat(); swap(T);
    printGrid(grids[T].t[SRC], grids[T].size);

    addDensity(); swap(Q);

    cout << "addForces!\n";
    //printGrid(grids[V].t[SRC], grids[V].size);
    addForces(V, gravity.y); swap(V);
    printGrid(grids[V].t[SRC], grids[V].size);

    cout << "compute divergence\n";
    divergence();
    printGrid(grids[D].t[0], grids[D].size);

    for (int i = 0; i < jacobiIterations; ++i) {
        runJacobi(P);
        swap(P);
    }
    //printGrid(grids[P].t[SRC], grids[P].size);

    gradient(U, vec2(1,0)); swap(U);
    gradient(V, vec2(0,1)); swap(V);

    printGrid(grids[V].t[SRC], grids[V].size);

    curStep++;
}

void FluidSimulator::advect(int g) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[g].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[U].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[V].t[SRC]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[g].t[SRC]);

    advectShader.begin();
    advectShader.setUniform("q.offset", grids[g].offset);
    advectShader.setUniform("q.size", grids[g].size);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    advectShader.end();
}

void FluidSimulator::addForces(int g, float f) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[g].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[T].t[SRC]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[Q].t[SRC]);

    forcesShader.begin();
    forcesShader.setUniform("f", f);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    forcesShader.end();
}

void FluidSimulator::addHeat() {
    glViewport(0, 0, (GLsizei) grids[T].size.x, (GLsizei) grids[T].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[T].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[T].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[H].t[SRC]);

    heatShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    heatShader.end();
}

void FluidSimulator::addDensity() {
    glViewport(0, 0, (GLsizei) grids[Q].size.x, (GLsizei) grids[Q].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[Q].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[Q].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[S].t[SRC]);

    smokeShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    smokeShader.end();
}

void FluidSimulator::divergence() {
    glViewport(0, 0, (GLsizei) grids[D].size.x, (GLsizei) grids[D].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[D].t[0], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[U].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[V].t[SRC]);

    divergenceShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    divergenceShader.end();
}

void FluidSimulator::runJacobi(int g){
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[g].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[D].t[0]);

    jacobiShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    jacobiShader.end();
}

void FluidSimulator::gradient(int g, vec2 delta) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            grids[g].t[DST], 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[P].t[SRC]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[g].t[SRC]);

    gradientShader.begin();
    gradientShader.setUniform("c.m", 1);
    gradientShader.setUniform("c.size", grids[g].size);
    gradientShader.setUniform("c.offset", grids[g].offset);
    gradientShader.setUniform("delta", delta);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    gradientShader.end();
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

    cout << "setting matrix\n";
    printGrid(grids[i].t[SRC], grids[i].size);

    CHECK_GL_ERRORS;
}