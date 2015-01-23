/*
 * Generating Complex Procedural Terrains Using the GPU
 *
 * Based on "Generating Complex Procedural Terrains Using the GPU",
 * Chapter 1, GPU Gems 3
 *
*/

#include <string>
#include <Aergia.h>
using aergia::io::GraphicsDisplay;
using aergia::io::resources::FrameBuffer;
using aergia::gui::Button;
#include <Algorithms.h>
using namespace aergia::algorithms;
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

GraphicsDisplay* gd;

int width = 100;
int height = 100;
int depth = 100;

void init(){
    // Save Marching Cubes lookup tables to textures
    unsigned short tableData[256*3];
    for (int i = 0; i < 256*3; ++i) {
        int ind = i/3;
        switch(i % 3){
            case 0: tableData[i] = MC::edgeTable[ind]; break;
            case 1: tableData[i] = MC::numTris[ind]; break;
            case 2: tableData[i] = MC::cellIndex[ind]; break;
            default:break;
        }
    }
    Texture table("tableData", 256, tableData);

    unsigned short triTable[1024];
    for (int i = 0; i < 820; ++i) {
        triTable[i] = MC::triTable[i];
    }
    Texture ttable("triTable", 1024, triTable, 1);

    unsigned char triData[512*3];
    for (int i = 0; i < 313*3; ++i) {
        int ind = i/3;
        triData[i] = MC::triVert[ind][i%3];
    }
    Texture vtable("vertTable", 512, triData);

    Shader densityShader;
    densityShader.loadFiles("density", "Shaders");
}

void render(){
    static int k = 0;
    if(!k){
        GLuint junkVAO;
        glGenVertexArrays(1, &junkVAO);

        glViewport(0, 0, width, height);
        glBindVertexArray(junkVAO);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 3, depth);
    }
}

int main(void){

    gd = GraphicsDisplay::create(800, 800, std::string("SimpleRTT"));
    gd->registerRenderFunc(render);

    init();

    gd->start();

    return 0;
}
