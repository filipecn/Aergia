/*
 * MACGrid.h
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

#include <OpenGL.h>
using namespace aergia::gl;

#include <Shader.h>
using aergia::graphics::rendering::Shader;

#include "ProceduralTexture.h"

#include <glm/glm.hpp>
using glm::ivec2;
using glm::vec2;

class MACGrid {
public:

    MACGrid();
    MACGrid(ivec2 size, vec2 offset, float* border = NULL);
    ~MACGrid();

    bool set(ivec2 size, vec2 offset, float* border = NULL);
    void swap();

    GLuint getSRCTexture();
    GLuint getSRCFBO();
    GLuint getDSTFBO();

    void print();
    void setData(const GLvoid *data);
    void reset();

    int SRC, DST;

    char name;
    vec2 offset;
    vec2 size;

    ProceduralTexture pt[2];

protected:

};