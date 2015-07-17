/*
 * MACGrid.cpp
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

#include "MACGrid.h"

#include <iostream>
using namespace std;

MACGrid::MACGrid() {
    SRC = 0;
    DST = 1;
}

MACGrid::MACGrid(ivec2 size, vec2 offset, float* border) {
    set(size, offset, border);
}

MACGrid::~MACGrid(){
}

bool MACGrid::set(ivec2 size, vec2 offset, float* border) {
    for (int i = 0; i < 2; ++i) {
        pt[i].set(size, GL_R32F, GL_RED, GL_FLOAT, border);
        pt[i].init();
    }

    this->offset = offset;
    this->size = size;
}

GLuint MACGrid::getSRCTexture() {
    return pt[SRC].texture;
}

GLuint MACGrid::getDSTFBO() {
    return pt[DST].fbo;
}

GLuint MACGrid::getSRCFBO() {
    return pt[SRC].fbo;
}

void MACGrid::swap(){
    int tmp = SRC;
    SRC = DST;
    DST = tmp;
}

void MACGrid::print() {
    cerr << "printing grid " << this->name << ":\n";
    cerr << pt[SRC] << endl;
}

void MACGrid::setData(const GLvoid *data) {
    pt[SRC].setData(data);
}

void MACGrid::reset() {
    pt[SRC].reset();
    pt[DST].reset();
}