/*
 * FluidSimulator.h
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

#ifndef FLUIDSIMULATOR_H
#define FLUIDSIMULATOR_H

#include "OpenGL.h"
using namespace aergia::gl;
#include <Shader.h>
using aergia::graphics::rendering::Shader;

#include <glm/glm.hpp>
#include <GL/gl.h>

using glm::vec2;
using glm::vec3;

namespace aergia {

    namespace algorithms {

        enum cellTypes {FLUID = 0, AIR, SOLID};
        enum gridTypes {U = 0, V, P, T, GRIDTYPESSIZE};

        class FluidSimulator {
        public:
            FluidSimulator();

            ~FluidSimulator();

            bool init(vec2 gridSize, float dx, float dt);
            void step();
            void render();

            vec2 gridSize;

        protected:
            GLuint fbo, junkVAO;
            GLuint *src, *dst;

            int curStep;

            float timestep;
            float cellSize;

            GLuint generateTexture(vec3 size);
            void swap();

            // SHADERS
            Shader texShader;
            Shader simpleShader;
            Shader advectShader;

            void printGrid(GLuint grid, vec2 size);
        };


    }
}
#endif
