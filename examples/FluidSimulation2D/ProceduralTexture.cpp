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

#include <iostream>
using namespace std;

ProceduralTexture::ProceduralTexture(){

}

ProceduralTexture::~ProceduralTexture(){
}

ProceduralTexture::ProceduralTexture(ivec2 size, GLint internalFormat,
                                     GLenum format, GLenum type, float *borderColor) {
    this->size = size;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    this->borderColor = borderColor;


}

bool ProceduralTexture::set(ivec2 size, GLint internalFormat,
                            GLenum format, GLenum type, float *borderColor) {
    this->size = size;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    this->borderColor = borderColor;

    //glGenVertexArrays(1, &junkVAO);
}

void ProceduralTexture::setData(GLvoid const *data) {
    int width = size.x;
    int height = size.y;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                 format, type, data);

    CHECK_GL_ERRORS;

    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    //                       texture, 0);
    //CHECK_GL_ERRORS;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    CHECK_GL_ERRORS;

    texShader.begin();
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    texShader.end();

    //cerr << "setting matrix\n";
    //print(SRC);

    CHECK_GL_ERRORS;
}


bool ProceduralTexture::init(int framebuffer) {
    if(framebuffer < 0){
        glGenFramebuffers(1, &fbo);
        framebuffer = fbo;
    }
    else fbo = framebuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

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

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) size.x, (GLsizei) size.y, 0,
                 format, type, 0);

    CHECK_GL_ERRORS;

    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    CHECK_GL_ERRORS;
    CHECK_FRAMEBUFFER;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texShader.loadFiles("screenTexture", "shaders");
    texShader.setUniform("tex", 0);

    return true;
}

void ProceduralTexture::reset() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ostream &operator<<(ostream &out, ProceduralTexture &pt) {
    int width = pt.size.x;
    int height = pt.size.y;

    float * data = NULL;

    data = new float[(int) (width * height)];

    for (int i(0); i < width; ++i) {
        for (int j(0); j < height; ++j) {
            data[j * width + i] = 0.0f;
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pt.texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);

    CHECK_GL_ERRORS;

    out << width << " " << height << endl;

    for (int j(height-1); j >= 0; --j) {
        for (int i(0); i < width; ++i) {
            out << (float) data[(int) (j * width + i)] << ",";
        }
        out << endl;
    }
    return out;
}
