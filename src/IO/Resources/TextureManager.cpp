/* 
 * TextureManager.cpp
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

#include "TextureManager.h"
using namespace aergia::io::resources;

#include <iostream>
using namespace std;

TextureManager::TextureManager(){
	// Load default textures
	string dir = Config::getInstance().DEFAULT_GUI_DIR;
	loadTextureAtlas(dir + string("defaultGUI_atlas"));
}

int TextureManager::getTextureID(string name){
	cout << "getTexture " << name << endl;
	auto it = name_id.find(name);
	if(it != name_id.end())
		return it->second;
	return -1;
}

vec2 TextureManager::texSize(int id){
	if(id < 0 || id >= textures.size())
		return vec2(0,0);
	return vec2( 	textures[id].size.x,
			textures[id].size.y);
}

vec4 TextureManager::texBBox(int id){
	if(id < 0 || id >= textures.size())
		return vec4(0,0,0,0);
	return textures[id].bbox;
}

bool TextureManager::loadTexture(string filename, string name){
	cout << "load texture " << filename << " " << name << endl;
	
	int width, height;

	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	if(!image)
		return false;

	TextureAttributes ta;
	glGenTextures(1, &ta.id);
	glBindTexture(GL_TEXTURE_2D, ta.id);
	
	cout << ta.id << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	//SOIL_free_image_data(image);

	ta.bbox = vec4(0,0,1,1);
	ta.size = vec3(width,height,0);
	
	cout << "inserting " << name << "," << textures.size() << " -> " << ta.id << endl;

	name_id.insert(pair<string,int>(name,textures.size()));

	cout << name_id.find(name)->second << endl;
	textures.push_back(ta);

	return true;
}

bool TextureManager::loadTextureAtlas(string atlas){
	ifstream is (atlas.c_str(), ifstream::binary);
	
	if(!is)
		return false;
	
	string path = atlas.substr(0, atlas.find_last_of("/\\"));

	cout << "PATH " << path << endl;
	
	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(is, pt);
	
	string imageFile = pt.get<std::string>("TextureAtlas.<xmlattr>.imagePath");

	cout << "file " << imageFile << endl;

	if(!loadTexture(path + "/" + imageFile, atlas))
		return false;

	int id = getTextureID(atlas);

	cout << "got texture id " << id << endl;
	cout << textures[id].id << endl;
	cout << textures[id].bbox << endl;
	cout << textures[id].size << endl;

	BOOST_FOREACH( ptree::value_type const& v, pt.get_child("TextureAtlas") ) {
		if(v.first == "sprite"){
			TextureAttributes ta;
			
			ta.id = textures[id].id;
			ta.size.x = v.second.get<int>("<xmlattr>.w");
			ta.size.y = v.second.get<int>("<xmlattr>.h");
			
			vec3 tSize = textures[id].size;

			cout << "atlas size " << tSize << endl;
			
			ta.bbox.z = (v.second.get<int>("<xmlattr>.w"))/tSize.x;
			ta.bbox.w = (v.second.get<int>("<xmlattr>.h"))/tSize.y;
			ta.bbox.x = (v.second.get<int>("<xmlattr>.x") - (int)(ta.bbox.z * v.second.get<float>("<xmlattr>.pX")))/tSize.x;
			ta.bbox.y = (v.second.get<int>("<xmlattr>.y") - (int)(ta.bbox.w * v.second.get<float>("<xmlattr>.pY")))/tSize.y;
			ta.bbox.z += ta.bbox.x;
			ta.bbox.w += ta.bbox.y;
			cout << ta.bbox << endl;
			
			name_id.insert(pair<string, int>(v.second.get<string>("<xmlattr>.n"), textures.size()));
			
			textures.push_back(ta);
		}
	}
	return true;
}

void TextureManager::applyTexture(int id, map<GLuint,GLuint> params){
	if(id < 0 || id >= textures.size())
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//cout << "id " << id << " -> " << textures[id].id << endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[id].id);
	
	for(auto param : params)
		glTexParameteri(GL_TEXTURE_2D, param.first, param.second);
}

int TextureManager::addTexture(string name, vec3 size, unsigned char* texels){
	
	name_id[name] = textures.size();
	
	images[textures.size()] = texels;
	
	TextureAttributes ta;
	glGenTextures(1, &ta.id);
	ta.size = size;

	glBindTexture(GL_TEXTURE_3D, ta.id);
	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, size.x, size.y, size.z, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);

	textures.push_back(ta);

	return textures.size()-1;
}
