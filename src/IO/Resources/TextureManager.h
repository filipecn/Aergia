/* 
 * TextureManager.h
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

#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <OpenGL.h>
#include <Config.h>
using aergia::common::Config;
#include <MathUtils.h>
using namespace aergia::math;

#include <SOIL.h>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec4;
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

namespace aergia {

	namespace io {

		namespace resources {

			class TextureManager {
				private:
					TextureManager();
				public:
					~TextureManager(){}

					inline static TextureManager& getInstance(){
						static TextureManager tm;
						return tm;
					}

					int getTextureID(string name);

					bool loadTexture(string filename, string name);
					bool loadTextureAtlas(string atlas);

					unsigned long addTexture(string name, vec3 size, unsigned char* texels);
					unsigned long addTexture(string name, vec2 size, unsigned char* texels);
					int addTexture(string name, int size, int channels, const unsigned char* texels);
					int addTexture(string name, int size, int channels, const unsigned short* texels);

					vec2 texSize(int id);
					vec4 texBBox(int id);

					void applyTexture(int id, map<GLuint,GLuint> params);

				private:
					struct TextureAttributes{
						GLuint id; // OpenGL ID
						vec4 bbox; // uv coordinades
						vec3 size; // size in pixels
					};

					map<string,unsigned long> name_id; // id = index of textures
					vector<TextureAttributes> textures;
					map<int,const unsigned char*> images;
			};

		}
	}
}

#endif
