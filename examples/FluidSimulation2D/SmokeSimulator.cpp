/*
 * SmokeSimulator.cpp
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

#include "SmokeSimulator.h"

#include <iostream>
using namespace std;

SmokeSimulator::SmokeSimulator(){
    curStep = 0;
    density = 0.1;

    rs = 10;
    rt = 10;
    ks = 1;
    kt = 0.8;
    Tamb = 273;

    bouyance_alpha = 10;
    bouyance_beta = 0.8;

    decayRate = 1.0;

    gravity = vec2(0.0,-10.0);
}

SmokeSimulator::~SmokeSimulator(){
}

int SmokeSimulator::getTexture(int g){
    return grids[g].getSRCTexture();
}

bool SmokeSimulator::init(ivec2 gridSize, float dx, float dt) {
    glGenFramebuffers(1, &fbo);

    this->gridSize = gridSize;
    this->timeStep = dt;
    this->cellSize = dx;

    grids[U].set(ivec2(gridSize.x+1,gridSize.y),vec2(-0.5,0.0)); grids[U].name = 'U';
    grids[V].set(ivec2(gridSize.x,gridSize.y+1),vec2(0.0,-0.5)); grids[V].name = 'V';

    float smokeBorder[] = {0,0,0,0};
    grids[Q].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0),smokeBorder); grids[Q].name = 'Q';
    grids[S].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0),smokeBorder); grids[S].name = 'S';

    float tBorder[] = {273,273,273,273};
    grids[T].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0),tBorder); grids[T].name = 'T';

    grids[H].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0)); grids[H].name = 'H';
    grids[D].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0)); grids[D].name = 'D';
    grids[P].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0),smokeBorder); grids[P].name = 'P';

    grids[C].set(ivec2(gridSize.x,gridSize.y),vec2(0.0,0.0),smokeBorder); grids[C].name = 'C';

    advectShader.loadFiles("advect", "shaders");
    advectShader.setUniform("u.m", 0);
    advectShader.setUniform("u.offset", grids[U].offset);
    advectShader.setUniform("u.size", grids[U].size);
    advectShader.setUniform("v.m", 1);
    advectShader.setUniform("v.offset", grids[V].offset);
    advectShader.setUniform("v.size", grids[V].size);
    advectShader.setUniform("q.m", 2);
    advectShader.setUniform("cellType.offset", grids[V].offset);
    advectShader.setUniform("cellType.size", grids[V].size);
    advectShader.setUniform("cellType.m", 3);
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
    forcesShader.setUniform("t.offset", grids[T].offset);
    forcesShader.setUniform("s.m", 2);
    forcesShader.setUniform("s.size", grids[Q].size);
    forcesShader.setUniform("s.offset", grids[Q].offset);
    forcesShader.setUniform("alpha", bouyance_alpha);
    forcesShader.setUniform("beta", bouyance_beta);
    forcesShader.setUniform("Tamb", Tamb);
    forcesShader.setUniform("dt", timeStep);
    forcesShader.setUniform("dx", cellSize);

    divergenceShader.loadFiles("divergence", "shaders");
    divergenceShader.setUniform("u.m", 0);
    divergenceShader.setUniform("u.size", grids[U].size);
    divergenceShader.setUniform("v.m", 1);
    divergenceShader.setUniform("v.size", grids[V].size);
    divergenceShader.setUniform("cellType.m", 2);
    divergenceShader.setUniform("cellType.size", grids[C].size);
    divergenceShader.setUniform("s", (density*cellSize)/timeStep);

    jacobiShader.loadFiles("jacobi", "shaders");
    jacobiShader.setUniform("x.m", 0);
    jacobiShader.setUniform("x.size", grids[P].size);
    jacobiShader.setUniform("b.m", 1);
    jacobiShader.setUniform("b.size", grids[D].size);
    jacobiShader.setUniform("cellType.m", 2);
    jacobiShader.setUniform("cellType.size", grids[C].size);
    jacobiShader.setUniform("alpha", -cellSize*cellSize);
    jacobiShader.setUniform("beta", (float)(1.0/4.0));

    gradientShader.loadFiles("gradient", "shaders");
    gradientShader.setUniform("p.m", 0);
    gradientShader.setUniform("p.size", grids[P].size);
    gradientShader.setUniform("s", timeStep/(density*cellSize));

    decayShader.loadFiles("decay", "shaders");
    decayShader.setUniform("q", 0);
    decayShader.setUniform("c", exp(-decayRate*timeStep));

    diffuseShader.loadFiles("diffuse", "shaders");
    diffuseShader.setUniform("q", 0);
    diffuseShader.setUniform("cellType", 1);
    diffuseShader.setUniform("s", (timeStep*kt)/(cellSize*cellSize));

    texShader.loadFiles("screenTexture", "shaders");
    texShader.setUniform("tex", 0);

    simpleShader.loadFiles("simple", "shaders");

    sShader.loadFiles("renderSmoke", "shaders");
    sShader.setUniform("tex", 0);

    rTShader.loadFiles("renderTemperature", "shaders");
    rTShader.setUniform("tex", 0);

    return true;
}

void SmokeSimulator::setGrid(int i, const GLvoid *data) {
    grids[i].setData(data);
    //grids[i].print();
}

void SmokeSimulator::reset() {
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SmokeSimulator::step() {
    static int k = 0;
    //if(k++ > 0)
    //  return;

    advect(V); grids[V].swap();
    advect(U); grids[U].swap();
    advect(Q); grids[Q].swap();
    advect(T); grids[T].swap();

    addHeat(); grids[T].swap();
    diffuse(T); grids[T].swap();
   // grids[T].print();
   // return;
    addDensity(); grids[Q].swap();

    addForces(V, gravity.y); grids[V].swap();

    divergence();

    grids[P].reset();

    for (int i = 0; i < jacobiIterations; ++i) {
        runJacobi(P);
        grids[P].swap();
    }

    gradient(U, vec2(1,0)); grids[U].swap();
    gradient(V, vec2(0,1)); grids[V].swap();

    decay(Q); grids[Q].swap();
    curStep++;
}

void SmokeSimulator::advect(int g) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[U].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[V].getSRCTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, grids[C].getSRCTexture());

    advectShader.begin();
        advectShader.setUniform("q.offset", grids[g].offset);
        advectShader.setUniform("q.size", grids[g].size);
        advectShader.setUniform("q.m", 2);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    advectShader.end();

    reset();
}

void SmokeSimulator::addForces(int g, float f) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[T].getSRCTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[Q].getSRCTexture());

    forcesShader.begin();
    forcesShader.setUniform("f", f);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    forcesShader.end();

    reset();
}

void SmokeSimulator::addHeat() {
    glViewport(0, 0, (GLsizei) grids[T].size.x, (GLsizei) grids[T].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[T].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[T].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[H].getSRCTexture());

    heatShader.begin();
    heatShader.setUniform("globalTime", curStep*timeStep);
    heatShader.setUniform("mouse", mouse);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    heatShader.end();

    reset();
}

void SmokeSimulator::diffuse(int g) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[C].getSRCTexture());

    diffuseShader.begin();
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    diffuseShader.end();

    reset();
}

void SmokeSimulator::addDensity() {
    glViewport(0, 0, (GLsizei) grids[Q].size.x, (GLsizei) grids[Q].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[Q].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[Q].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[S].getSRCTexture());

    smokeShader.begin();
    smokeShader.setUniform("mouse", mouse);
    smokeShader.setUniform("globalTime", curStep*timeStep);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    smokeShader.end();

    reset();
}

void SmokeSimulator::divergence() {
    glViewport(0, 0, (GLsizei) grids[D].size.x, (GLsizei) grids[D].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[D].getSRCFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[U].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[V].getSRCTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[C].getSRCTexture());

    divergenceShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    divergenceShader.end();

    reset();
}

void SmokeSimulator::runJacobi(int g){
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[D].getSRCTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grids[C].getSRCTexture());

    jacobiShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    jacobiShader.end();

    reset();
}

void SmokeSimulator::gradient(int g, vec2 delta) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[P].getSRCTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());

    gradientShader.begin();
    gradientShader.setUniform("c.m", 1);
    gradientShader.setUniform("c.size", grids[g].size);
    gradientShader.setUniform("c.offset", grids[g].offset);
    gradientShader.setUniform("delta", delta);
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    gradientShader.end();

    reset();
}

void SmokeSimulator::decay(int g) {
    glViewport(0, 0, (GLsizei) grids[g].size.x, (GLsizei) grids[g].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER,  grids[g].getDSTFBO());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grids[g].getSRCTexture());

    decayShader.begin();
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    decayShader.end();

    reset();
}

