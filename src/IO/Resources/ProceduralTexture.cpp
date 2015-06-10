/*
 * ProceduralTexture.cpp
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

#include "ProceduralTexture.h"
using namespace aergia::io::resources;

#include <iostream>
using namespace std;

ProceduralTexture::ProceduralTexture(){

}

ProceduralTexture::~ProceduralTexture(){
}

ProceduralTexture::ProceduralTexture(vec3 size, GLint internalFormat,
        GLenum format, GLenum type, float *borderColor = NULL) {
    this->size = size;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    this->borderColor = borderColor;

    glGenVertexArrays(1, &junkVAO);
}

void ProceduralTexture::setData(GLvoid const *data) {

}


bool ProceduralTexture::init(int framebuffer = -1) {
    if(framebuffer < 0){
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        framebuffer = fbo;
    }

    if(size.z == 0){

    }
    else {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_3D, texture);

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

        glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, (GLsizei) size.x,
                (GLsizei) size.y, (GLsizei) size.z, 0,
                format, type, 0);
    }

    CHECK_GL_ERRORS;

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    CHECK_GL_ERRORS;
    CHECK_FRAMEBUFFER;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}

ostream &ProceduralTexture::operator<<(ostream &out) {
    int width = size.x;
    int height = size.y;
    int depth = size.z;

    float * data = NULL;

    if(depth != 0) {
        data = new float[(int) (size.x * size.y * size.z)];

        for (int i(0); i < width; ++i) {
            for (int j(0); j < height; ++j) {
                for (int k(0); k < depth; ++k) {
                    data[(int) (k * (width * height) + j * width + i)] = 0.0f;
                }
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, texture);
        glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, data);

        CHECK_GL_ERRORS;

        out << "printing grid:\n";
        out << width << " " << height << " " << depth << endl;

        for (int i(0); i < width; ++i) {
            for (int j(0); j < height; ++j) {
                for (int k(0); k < depth; ++k) {
                    out << (float) data[(int) (k * (width * height) + j * width + i)] << ",";
                }
                out << endl;
            }
            out << endl;
        }
    }
    return out;
}
