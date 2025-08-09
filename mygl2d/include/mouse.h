#ifndef MOUSE_H
#define MOUSE_H

#include <stdlib.h>
#include <stdbool.h>

#include "glfw.h"

typedef struct Mouse Mouse;

struct Mouse {
	int x,y;
	int isLeftButtonDown;
	int isRightButtonDown;
};

Mouse *Mouse_New();
void Mouse_Free(Mouse **mouse);
void Mouse_Update(Mouse *mouse);

#endif /* MOUSE_H */