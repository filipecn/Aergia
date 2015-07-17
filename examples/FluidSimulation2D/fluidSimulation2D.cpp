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

#include <ctime>

#include <glm/glm.hpp>
using glm::ivec2;
using glm::vec2;
using glm::vec3;

#include <iostream>
using namespace std;

#include "ProceduralTexture.h"
#include "MACGrid.h"
#include "SmokeSimulator.h"

GraphicsDisplay* gd;
SmokeSimulator s;

Shader screen;
int curGrid;

int width = 512;
int height = 512;
float maxTemp = 400.0;

void init(){
    screen.loadFiles("renderScreen", "shaders");
    screen.setUniform("S",0);
    screen.setUniform("T",1);
    screen.setUniform("H",2);
    screen.setUniform("maxTemp", maxTemp);

    s.mouse = vec2(0.5,0.5);
    cerr << "mouse >>>>  " << s.mouse << endl;


    s.init(ivec2(width,height), 0.1, 0.001);
    s.jacobiIterations = 80;


    int low = (int) (height*0.40), top = (int) (height*0.8);
    int l = (int) (width*0.3), r = (int) (width*0.7);

    // INITIATE C
    {
        int w = width, h = height+1;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if(j > w*0.1 && j < w*0.8 && i > h*0.1 && i < h*0.9)
                    iuImg[i*w + j] = 0.0f;
                else iuImg[i*w + j] = 1.0;
            }
        }
        cerr << "SET C\n";
        s.setGrid(gridTypes::C, iuImg);
    }

    // INITIATE V
    {
        int w = width, h = height+1;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                //if(i >= 400 && i <= 5)
                iuImg[i*w + j] = -1000.0f;
                //else iuImg[i*w + j] = 0.0;
            }
        }
        cerr << "SET V\n";
        //s.setGrid(gridTypes::V, iuImg);
    }

    // INITIATE S
    {
        int w = width, h = height;
        GLfloat iuImg[w*h];

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                //if(j > w*0.45 && j < w*0.55 && i < h*0.8 && i > h*0.75)
                if(i < h*0.15 && i > h*0.1 && j > w*0.45 && j < w*0.55)
                //if(i < h*0.15 && j > w*0.45 && j < w*0.55)
                    iuImg[i*w + j] = 0.15;
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
                if(i <= top && i >= low && j >= l && j <= r)
                //if(i <= 10 && i >= 7)
                    iuImg[i*w + j] = 1.0;
                else iuImg[i*w + j] = 0.0;
            }
        }
        cerr << "SET Q\n";
        //s.setGrid(gridTypes::Q, iuImg);
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
                //if(i < h*0.1)

            //    if(i < h*0.15 && i > h*0.1 && j > w*0.45 && j < w*0.55)
              //      iuImg[i*w + j] = maxTemp;
            }
        }
        cerr << "SET H\n";
        s.setGrid(gridTypes::H, iuImg);
    }
}

void render(){
    s.step();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, (GLuint) s.getTexture(gridTypes::Q));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, (GLuint) s.getTexture(gridTypes::T));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, (GLuint) s.getTexture(gridTypes::H));

    screen.begin();
    screen.setUniform("mode", curGrid);
    screen.setUniform("mouse", s.mouse);
    screen.setUniform("globalTime", s.timeStep*s.curStep);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    screen.end();
}

void mouseButton(int button, int action){
    s.step();
}

void keyboard(int key, int action){
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        gd->stop();
    if(key == GLFW_KEY_S && action == GLFW_PRESS)
        curGrid = 0;//gridTypes::Q;
    if(key == GLFW_KEY_T && action == GLFW_PRESS)
        curGrid = 1;//gridTypes::T;
    if(key == GLFW_KEY_H && action == GLFW_PRESS)
        curGrid = 2;//gridTypes::H;
}

void mouse(double x, double y){
    s.mouse = vec2((float)x/(float)width,(float)(height - y)/(float)height);

}

int main(void){

    gd = GraphicsDisplay::create(width, height, std::string("Smoke2D"));
    gd->registerRenderFunc(render);
    gd->registerButtonFunc(mouseButton);
    gd->registerKeyFunc(keyboard);
    gd->registerMouseFunc(mouse);
    init();

    gd->start();

    return 0;
}
