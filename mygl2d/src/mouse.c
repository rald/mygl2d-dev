#include "mouse.h"

Mouse *Mouse_New() {
	Mouse *mouse=malloc(sizeof(*mouse));
	mouse->x=0;
	mouse->y=0;
	mouse->isLeftButtonDown=false;
	mouse->isRightButtonDown=false;
	return mouse;
}

void Mouse_Free(Mouse **mouse) {
	free(*mouse);
	(*mouse)=NULL;
}

void Mouse_Update(Mouse *mouse) {
	glfwPollEvents();

	glfwGetMousePos(&mouse->x,&mouse->y);

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
		mouse->isLeftButtonDown=true;
	} else {
		mouse->isLeftButtonDown=false;
	}

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS) {
		mouse->isRightButtonDown=true;
	} else {
		mouse->isRightButtonDown=false;
	}
}

