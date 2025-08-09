#include <stdbool.h>

#include "miniaudio.h"
#include "glfw.h"

#include "mygl2d.h"
#include "mouse.h"

#define GAME_TITLE "mygl2d"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

bool quit=false;

ma_result result;
ma_engine engine;

Mouse *mouse=NULL;

bool hold=false;

GLuint cursor=0;

GLuint spin=0;

Frame frames[]={
	{ 0, 0,16,16},
	{16, 0,16,16},
	{32, 0,16,16},
	{48, 0,16,16},
};

Animation spinAnim;

bool inrect(int x,int y,int rx,int ry,int w,int h) {
	return x>=rx && x<=rx+w && y>=ry && y<=ry+h;
}

bool incirc(int x,int y,int cx,int cy,int cr) {
	return (cx-x)*(cx-x) + (cy-y)*(cy-y) < cr*cr;
}

void rotatePoint(float cx, float cy, float angleInRads,float *x,float *y) {
    float s = sin(angleInRads);
    float c = cos(angleInRads);

    // Translate point back to origin
    *x -= cx;
    *y -= cy;

    // Rotate point
    float xnew = *x * c - *y * s;
    float ynew = *x * s + *y * c;

    // Translate point back
    *x = xnew + cx;
    *y = ynew + cy;
}

const float TWO_PI = 2 * M_PI;
float clampRadians(float angle) {
    if (angle < -TWO_PI || angle > TWO_PI) {
        const float cycles = angle >= 0 ? floor(angle / TWO_PI) : ceil(angle / TWO_PI);
        return angle - cycles * TWO_PI;
    }
    return angle;
}

void update(double deltaTime) {
	Mouse_Update(mouse);
	updateAnimation(&spinAnim,deltaTime);
}

void draw() {
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

	clearScreen();

	if(mouse->isLeftButtonDown && incirc(mouse->x,mouse->y,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,32)) {
		glColor4ub(255,255,255,255);
		fillCircle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,32);
		if(!hold) {
			ma_engine_play_sound(&engine, "sfx/beep.mp3", NULL);
			hold=true;
		}
	} else {
		glColor4ub(255,255,255,255);
		drawCircle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,32);
		hold=false;
	}

	if(mouse->x<0) { mouse->x=0; glfwSetMousePos(mouse->x,mouse->y); }
	if(mouse->y<0) { mouse->y=0; glfwSetMousePos(mouse->x,mouse->y); }
	if(mouse->x>SCREEN_WIDTH-16) { mouse->x=SCREEN_WIDTH-16; glfwSetMousePos(mouse->x,mouse->y); }
	if(mouse->y>SCREEN_HEIGHT-16) { mouse->y=SCREEN_HEIGHT-16; glfwSetMousePos(mouse->x,mouse->y); }

	drawTargaWithClip(spin,64,16,frames[spinAnim.currentFrame],0,0);

	drawTarga(cursor,mouse->x,mouse->y);


/*
	glColor4ub(255,255,255,255);
	drawRect(1,1,SCREEN_WIDTH-1,SCREEN_HEIGHT-1);
*/


	glFlush();
	glfwSwapBuffers();
}

int main(int argc, char** argv) {
	glfwInit();

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
	glfwOpenWindow(SCREEN_WIDTH,SCREEN_HEIGHT,0, 0, 0, 0, 0, 0,GLFW_FULLSCREEN);
	glfwSetWindowTitle(GAME_TITLE);
	glfwDisable(GLFW_MOUSE_CURSOR);

	mouse=Mouse_New();

	cursor=loadTarga("images/cursor.tga");
	spin=loadTarga("images/spin.tga");

	spinAnim=(Animation){frames,4,0,0.1f,0.0f};

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return -1;
    }

	double lastTime=0;
	while(!quit) {
		double currentTime=glfwGetTime();
		double deltaTime=currentTime-lastTime;
		lastTime=currentTime;

		update(deltaTime);

		draw();

		quit = glfwGetKey(GLFW_KEY_ESC) | !glfwGetWindowParam(GLFW_OPENED);
	}

	ma_engine_uninit(&engine);

	Mouse_Free(&mouse);

	return 0;
}



