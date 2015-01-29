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
    s.init(vec2(width,height), 0.1, 0.1);
}

void render(){
    s.step();
    s.render();
}

int main(void){

    gd = GraphicsDisplay::create(width, height, std::string("PTG"));
    gd->registerRenderFunc(render);

    init();

    gd->start();

    return 0;
}
