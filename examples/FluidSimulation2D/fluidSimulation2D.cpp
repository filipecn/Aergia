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

int width = 50;
int height = 50;

void init(){

    s.init(vec2(width,height), 0.1, 0.001);

    // INITIATE U
    {
        int w = width+1, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if(i >= 10 && i <= 40 && j >= 10 & j <= 40)
                    iuImg[i*w + j] = 0.5;
                else iuImg[i*w + j] = 0.0;
            }
        }

        s.setGrid(gridTypes::U, iuImg);
    }

    // INITIATE V
    {
        int w = width, h = height+1;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if(i >= 10 && i <= 40 && j >= 10 & j <= 40)
                    iuImg[i*w + j] = 0.5;
                else iuImg[i*w + j] = 0.0;
            }
        }

        s.setGrid(gridTypes::V, iuImg);
    }

    // INITIATE Q
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if(i >= 10 && i <= 40 && j >= 10 & j <= 40)
                    iuImg[i*w + j] = 1;
                else iuImg[i*w + j] = 0.0;
            }
        }

        s.setGrid(gridTypes::Q, iuImg);
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

    gd = GraphicsDisplay::create(width, height, std::string("PTG"));
    gd->registerRenderFunc(render);
    gd->registerButtonFunc(mouseButton);
    init();

    gd->start();

    return 0;
}
