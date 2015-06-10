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

#include <glm/glm.hpp>
using glm::ivec2;
using glm::vec3;

#include <iostream>
using namespace std;

//#include "FluidSimulator.h"
//using namespace aergia::algorithms;

#include "ProceduralTexture.h"
#include "MACGrid.h"
#include "SmokeSimulator.h"

GraphicsDisplay* gd;
SmokeSimulator s;


int width = 500;
int height = 500;

void init(){

    s.init(ivec2(width,height), 0.1, 0.001);
    s.jacobiIterations = 80;

    int l = (int) (width*0.2), r = (int) (width*0.4);

    // INITIATE V
    {
        int w = width, h = height+1;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                //if(i >= 400 && i <= 5)
                iuImg[i*w + j] = -30.0f;
                //else iuImg[i*w + j] = 0.0;
            }
        }
        cerr << "SET V\n";
        s.setGrid(gridTypes::V, iuImg);
    }

    // INITIATE Q
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                //if(i >= l && i <= r && j >= l & j <= r)
                if(i <= 5 && i >= 4)
                    iuImg[i*w + j] = 1.0;
                else iuImg[i*w + j] = 0.0;
            }
        }
        cerr << "SET S\n";
        //s.setGrid(gridTypes::S, iuImg);
    }

    // INITIATE Q
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                //if(i >= l && i <= r && j >= l & j <= r)
                if(i <= 500 && i >= 400)
                    iuImg[i*w + j] = 1.0;
                else iuImg[i*w + j] = 0.0;
            }
        }
        cerr << "SET Q\n";
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
        cerr << "SET T\n";
        s.setGrid(gridTypes::T, iuImg);
    }
    // INITIATE H
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = h-1; i >= 0; --i) {
            for (int j = w-1; j >= 0; --j) {
                iuImg[i*w + j] = 273;
                if(i == 0)
                    iuImg[i*w + j] = 500;
            }
        }
        cerr << "SET H\n";
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
/*
    MACGrid mg(ivec2(width+1,height), vec2(-0.5,-0.5));

    ProceduralTexture test(ivec2(width+1,height), GL_R32F, GL_RED, GL_FLOAT);
    test.init();
    cout << test << endl;

    {
        int w = width+1, h = height;
        GLfloat iuImg[w*h];

        for (int i = h-1; i >= 0; --i) {
            for (int j = w-1; j >= 0; --j) {
                iuImg[i*w + j] = 273;
                if(i == 0)
                  iuImg[i*w + j] = 500;
            }
        }

        test.setData(iuImg);
    }
    cout << test << endl;
    //test.print(1);

    SmokeSimulator sm;
    sm.init(ivec2(width+1,height),0,0);
*/
    gd->start();

    return 0;
}
