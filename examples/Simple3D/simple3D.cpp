/* 
 * SIMPLE3D EXAMPLE
 *
 * Simple example of setting up a OpenGL window display, 
 * with a cartesian grid and mouse trackball control.
 * 
*/

#include <string>
#include <Aergia.h>

using aergia::io::GraphicsDisplay;
using aergia::graphics::helpers::Grid;

GraphicsDisplay* gd = NULL;

int main(void){
	
	gd = GraphicsDisplay::create(800, 800, std::string("Simple3D"));
	
	gd->add(new Grid(5));

	gd->start();

	return 0;
}
