/*
 * TerrainGenerator.cpp
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

#include "TerrainGenerator.h"
using namespace aergia::algorithms;

#include <iostream>
#include <GL/gl.h>

using namespace std;

TerrainGenerator::TerrainGenerator(){
}

TerrainGenerator::~TerrainGenerator(){
}

void TerrainGenerator::generateGrid(int G, vec3 size, GLint internalFormat, GLenum format,
        GLenum type, float *borderColor = NULL){

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &grids[G].t);
    glBindTexture(GL_TEXTURE_3D, grids[G].t);

    if(borderColor == NULL) {
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    else {
        glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    }
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

/*
    switch (numComponents) {
        case 1:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, depth, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, width, height, depth, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }
*/
    glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, (GLsizei) size.x, (GLsizei) size.y, (GLsizei) size.z, 0,
            format, type, 0);

    CHECK_GL_ERRORS;

    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, grids[G].t, 0);

    CHECK_GL_ERRORS;
    CHECK_FRAMEBUFFER;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    grids[G].size = size;
    grids[G].internalFormat = internalFormat;
    grids[G].format = format;
    grids[G].type = type;
}

void TerrainGenerator::printGrid(GLuint grid, vec3 size){
    int width = size.x;
    int height = size.y;
    int depth = size.z;

    float * data = new float[(int)(size.x*size.y*size.z)];
    for(int i(0); i < width; ++i) {
        for(int j(0); j < height; ++j) {
            for(int k(0); k < depth; ++k) {
                data[(int)(k*(width*height) + j*width + i)] = 0.0f;
            }
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, grid);
    glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, data);

    CHECK_GL_ERRORS;

    cout << "printing grid:\n";
    cout << width << " " << height << " " << depth << endl;

    for(int i(0); i < width; ++i) {
        for(int j(0); j < height; ++j) {
            for(int k(0); k < depth; ++k) {
                cout << (float)data[(int)(k*(width*height) + j*width + i)] << ",";
            }
            cout << endl;
        }
        cout << endl;
    }
}

bool TerrainGenerator::init(vec3 gridSize) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glGenVertexArrays(1, &junkVAO);
    short positions[] = {
            -1, -1,
            1, -1,
            -1,  1,
            1,  1,
    };
    GLuint vbo;
    GLsizeiptr size = sizeof(positions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    this->gridSize = gridSize;

    generateGrid(0, vec3(gridSize.x,gridSize.y,gridSize.z), GL_R16F, GL_RED, GL_HALF_FLOAT);
    grids[0].m = 0;

    simpleNoiseShader.loadFiles("simpleNoise", "shaders");

    texShader.loadFiles("texture", "shaders");
    texShader.setUniform("tex", 0);

    printGrid(grids[0].t, grids[0].size);
    simpleNoise(0);
    printGrid(grids[0].t, grids[0].size);
    CHECK_GL_ERRORS;
    return true;
}

void TerrainGenerator::render(){
    glViewport(0, 0, (GLsizei) grids[0].size.x, (GLsizei) grids[0].size.y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, grids[0].t);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texShader.begin();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
    texShader.end();
}

void TerrainGenerator::simpleNoise(int G) {
    glViewport(0, 0, (GLsizei) grids[G].size.x, (GLsizei) grids[G].size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, grids[G].t, 0);

    simpleNoiseShader.begin();
        //glBindVertexArray(junkVAO);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 3, (GLsizei) grids[G].size.z);
        //glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei) grids[G].size.z);
    simpleNoiseShader.end();
}

void TerrainGenerator::setGrid(int i, const GLvoid *data) {

}
