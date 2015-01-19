/* 
 * SIMPLEGUI EXAMPLE
 *
 * Simple example of setting up some basic GUI elements.
 * 
*/

#include <string>
#include <Aergia.h>
using aergia::io::GraphicsDisplay;
using aergia::gui::Button;
using aergia::graphics::helpers::Box;
#include <glm/glm.hpp>
using glm::vec3;

GraphicsDisplay* gd = NULL;
Box* box;

void buttonClick(int button){
	box->color = vec3(0.5,0.5,0.5);	
}

int main(void){

	gd = GraphicsDisplay::create(800, 800, std::string("SimpleGUI"));

	box = new Box(vec3(0,0,0),vec3(1,1,1));
	box->color = vec3(1,1,1);
	gd->add(box);

	Button* button = new Button(std::string("Button"), 0.1, 0.1);
	button->registerMouseClickFunc(buttonClick);
	gd->add(button);

	gd->start();

	return 0;
}
