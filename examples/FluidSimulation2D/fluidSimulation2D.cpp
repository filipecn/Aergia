/*
 * 2D Fluid Simulation Using the GPU
 *
 * Based on "Fast Fluid Dynamics Simulation on the GPU",
 * Chapter 38, GPU Gems 2
 *
*/

#include <string>
#include <Aergia.h>
using aergia::io::GraphicsDisplay;
#include <Algorithms.h>
using namespace aergia::algorithms;
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include <iostream>
using namespace std;

GraphicsDisplay* gd;
FluidSimulator s;

int width = 10;
int height = 10;

void init(){

    s.init(vec2(width,height), 0.1, 0.0001);
    s.jacobiIterations = 80;

    int l = (int) (width*0.2), r = (int) (width*0.4);

    // INITIATE Q
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if(i >= l && i <= r && j >= l & j <= r)
                    iuImg[i*w + j] = 1;
                else iuImg[i*w + j] = 0.0;
            }
        }

        s.setGrid(gridTypes::Q, iuImg);
    }

    // INITIATE T
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                iuImg[i*w + j] = 273;
            }
        }

        s.setGrid(gridTypes::T, iuImg);
    }
    // INITIATE H
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                iuImg[i*w + j] = 273;
            }
        }

        s.setGrid(gridTypes::H, iuImg);
    }

}

void render(){
    s.step();
    s.render();
}

void mouseButton(int button, int action){
    s.step();
}

int main(void){

    gd = GraphicsDisplay::create(width, height, std::string("Smoke2D"));
    gd->registerRenderFunc(render);
    gd->registerButtonFunc(mouseButton);
    init();

    gd->start();

    return 0;
}
