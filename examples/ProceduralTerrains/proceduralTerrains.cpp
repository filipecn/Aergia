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
TerrainGenerator tg;
//FrameBuffer fb;
//Shader densityShader;
//Shader texShader;

int width = 8;
int height = 8;
int depth = 8;

/*
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

    //densityShader.loadFiles("density", "shaders");

    texShader.loadFiles("texture", "shaders");

   // fb = FrameBuffer(vec2(width,height));


}

void render(){
    static int k = 0;

    if(!k){
        //densityShader.begin();
        //fb.enable();

        //GLuint junkVAO;
        //glGenVertexArrays(1, &junkVAO);

        //glViewport(0, 0, width, height);
        //glBindVertexArray(junkVAO);
        //glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 3, depth);

     //   fb.disable();
        //densityShader.end();
    }

    //texShader.begin();
    //texShader.setUniform("tex", 0);
/*
    GLuint junkVAO;
    glGenVertexArrays(1, &junkVAO);

    glViewport(0, 0, width, height);
    glDrawArrays( GL_TRIANGLES, 0, 3 );
*/ /*
    // Bind framebuffer with texture
    // glClearColor(RED)
    // glClear(GL_COLOR_BUFFER_BIT)
    // unbind framebuffer -> uses the default one
    // Then render screen quad of your attached texture






    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);

    // Setup Texture:
    GLuint texture2D;
    {
        glGenTextures(1, &texture2D);
        glBindTexture(GL_TEXTURE_2D, texture2D);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        CHECK_GL_ERRORS;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture2D, 0);
    CHECK_FRAMEBUFFER;


    glViewport(0, 0, width, height);
    glClearColor(1,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERRORS;

    // NOW Render texture2D as a screen quad.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2D);

    texShader.begin();
    texShader.setUniform("u_textureUnit", 0);


    const int kAttribIndex_positions = 0;
    const int kAttribIndex_texCoords = 1;

    GLuint screenQuadVao, screenQuadVertBuffer, screenQuadIndexBuffer;

    glGenVertexArrays(1, &screenQuadVao);
    glBindVertexArray(screenQuadVao);
    glEnableVertexAttribArray(kAttribIndex_positions);
    glEnableVertexAttribArray(kAttribIndex_texCoords);
    glBindVertexArray(0);



    glBindVertexArray(screenQuadVao);

    glGenBuffers(1, &screenQuadVertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, screenQuadVertBuffer);

    float vertexData[] = {
            //  Position      Texture-Coords
            -1.0f,  1.0f,    0.0f, 1.0f,   // Top-left
            1.0f,  1.0f,    1.0f, 1.0f,   // Top-right
            1.0f, -1.0f,    1.0f, 0.0f,   // Bottom-right
            -1.0f, -1.0f,    0.0f, 0.0f    // Bottom-left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    int elementsPerVertex;
    int stride;
    int offsetToFirstElement;

    //-- Position Data:
    elementsPerVertex = 2;
    stride = 4*sizeof(float);
    offsetToFirstElement = 0;
    glVertexAttribPointer(kAttribIndex_positions, elementsPerVertex, GL_FLOAT,
            GL_FALSE, stride, reinterpret_cast<void *>(offsetToFirstElement));

    //-- Texture Coordinate Data:
    elementsPerVertex = 2;
    stride = 4*sizeof(float);
    offsetToFirstElement = 2*sizeof(float);
    glVertexAttribPointer(kAttribIndex_texCoords, elementsPerVertex, GL_FLOAT,
            GL_FALSE, stride, reinterpret_cast<void *>(offsetToFirstElement));


    glGenBuffers(1, &screenQuadIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuadIndexBuffer);

    GLushort indices [] = {
            3,2,1, 0,3,1
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    CHECK_GL_ERRORS;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    CHECK_GL_ERRORS;


    //  texShader.end();
    k++;
}
*/

void init(){
    tg.init(vec3(width,height,depth));
}

void render(){
    tg.render();
}


int main(void){
    gd = GraphicsDisplay::create(width, height, std::string("PTG"));
    gd->registerRenderFunc(render);

    init();

    gd->start();

    return 0;
}
