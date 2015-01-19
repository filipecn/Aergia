/* 
 * SIMPLEGUI EXAMPLE
 *
 * Simple example of rendering to a texture.
 * 
*/

#include <string>
#include <Aergia.h>
using aergia::io::GraphicsDisplay;
using aergia::io::resources::FrameBuffer;
using aergia::gui::Button;
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

GraphicsDisplay* gd;
FrameBuffer fb;

void render(){
	fb.enable();
	
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);

//	fb.disable();
}

void buttonClick(int button){
}

int main(void){
	
	gd = GraphicsDisplay::create(800, 800, std::string("SimpleRTT"));
	gd->registerRenderFunc(render);
		
	Button* button = new Button(std::string("Button"), 0.1, 0.1);
	button->registerMouseClickFunc(buttonClick);
	gd->add(button);
	
	fb = FrameBuffer(vec2(800,800));

	gd->start();

	return 0;
}
