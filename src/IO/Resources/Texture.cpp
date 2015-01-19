/* 
 * Texture.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 FilipeCN
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

#include "Texture.h"
using namespace aergia::io::resources;

Texture::Texture(){
	name = string();
	id = -1;
}

Texture::Texture(string name){
	this->name = name;
	this->id = TextureManager::getInstance().getTextureID(name);

	parameters[GL_TEXTURE_WRAP_S] = GL_CLAMP_TO_EDGE;
	parameters[GL_TEXTURE_WRAP_T] = GL_CLAMP_TO_EDGE;
	parameters[GL_TEXTURE_MIN_FILTER] = GL_LINEAR;
	parameters[GL_TEXTURE_MAG_FILTER] = GL_LINEAR;
}

vec2 Texture::getSize(){
	return TextureManager::getInstance().texSize(id);
}

vec4 Texture::getBBox(){
	return TextureManager::getInstance().texBBox(id);
}

void Texture::apply(){
	TextureManager::getInstance().applyTexture(id, parameters);
}
