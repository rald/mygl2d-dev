#ifndef MYGL2D_H
#define MYGL2D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>

#include "glfw.h"

#pragma pack(push, 1) // Ensure no padding
typedef struct {
    unsigned char id_length;
    unsigned char color_map_type;
    unsigned char image_type;
    unsigned short color_map_index;
    unsigned short color_map_length;
    unsigned char color_map_size;
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char bits_per_pixel;
    unsigned char image_descriptor;
} TGAHeader;
#pragma pack(pop)

typedef struct {
    int x, y, w, h;
} Frame;

typedef struct {
    Frame* frames;
    int frameCount;
    int currentFrame;
    float frameDuration;    // seconds per frame
    float elapsedTime;
} Animation;

void clearScreen(void);
void drawPoint(int x, int y);
void drawLine(int x1, int y1, int x2, int y2);
void drawRect(int x, int y, int w, int h);
void fillRect(int x, int y, int w, int h);
void drawCircle(int cx, int cy, int r);
void fillCircle(int cx, int cy, int r);
void drawOval(int cx, int cy, int rx, int ry);
void fillOval(int cx, int cy, int rx, int ry);
void drawPolygon(const int* points, int num_points);
void fillPolygon(const int* points, int num_points);

GLuint loadTarga(const char* filename);
void drawTarga(GLuint textureID, int x, int y);

void updateAnimation(Animation* animation, float deltaTime);
void drawTargaWithClip(GLuint texture, int texWidth, int texHeight, Frame clip, float posX, float posY);

#endif /* MYGL2D_H */

