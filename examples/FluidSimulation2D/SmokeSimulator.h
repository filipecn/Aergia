/*
 * SmokeSimulator.h
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

#pragma once

#include "OpenGL.h"
using namespace aergia::gl;
#include <Shader.h>
using aergia::graphics::rendering::Shader;
#include "MACGrid.h"

#include <glm/glm.hpp>
#include <GL/gl.h>

using glm::ivec2;
using glm::vec3;

enum cellTypes {FLUID = 0, AIR, SOLID, CELLTYPESSIZE};
enum gridTypes {U = 0, V, P, D, Q, S, T, H, C, GRIDTYPESSIZE};

class SmokeSimulator {
public:
    SmokeSimulator();
    ~SmokeSimulator();

    bool init(ivec2 gridSize, float dx, float dt);
    void setGrid(int i, const GLvoid *data);
    void step();
    int getTexture(int g);

    ivec2 gridSize;
    vec2 mouse;

    int jacobiIterations;

    int curStep;

    float timeStep;

protected:
    MACGrid grids[GRIDTYPESSIZE];

    float cellSize;
    float density;
    vec2 gravity;
    float bouyance_alpha;
    float bouyance_beta;
    float decayRate;

    float Tamb;
    float rt, rs;
    float ks, kt;

    GLuint fbo;

    // SHADERS
    Shader texShader;
    Shader simpleShader;
    Shader advectShader;
    Shader forcesShader;
    Shader divergenceShader;
    Shader jacobiShader;
    Shader gradientShader;
    Shader heatShader;
    Shader smokeShader;
    Shader diffuseShader;
    Shader decayShader;

    Shader sShader;
    Shader rTShader;

    // SIMULATION
    void advect(int g);
    void addForces(int g, float f);
    void addHeat();
    void addDensity();
    void divergence();
    void runJacobi(int g);
    void gradient(int g, vec2 delta);
    void decay(int g);
    void diffuse(int g);

    void reset();
};
