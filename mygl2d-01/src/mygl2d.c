#include "mygl2d.h"

void clearScreen(void) {
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT);
}

void drawPoint(int x, int y) {
    glPointSize(1);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawLine(int x1, int y1, int x2, int y2) {
  glBegin(GL_LINES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glEnd();
}

void drawRect(int x, int y, int w, int h) {
  glBegin(GL_LINE_LOOP);
  glVertex2i(x, y);
  glVertex2i(x + w, y);
  glVertex2i(x + w, y + h);
  glVertex2i(x, y + h);
  glEnd();
}

void fillRect(int x, int y, int w, int h) {
  glBegin(GL_QUADS);
  glVertex2i(x, y);
  glVertex2i(x + w, y);
  glVertex2i(x + w, y + h);
  glVertex2i(x, y + h);
  glEnd();
}

void drawCircle(int cx, int cy, int r) {
  glBegin(GL_LINE_LOOP);
  int num_segs = 100;
  for (int i = 0; i < num_segs; ++i) {
    float theta = 2.0f * 3.1415926f * (float)(i) / (float)(num_segs);
    int x = (int)(round(cx + r * cos(theta)));
    int y = (int)(round(cy + r * sin(theta)));
    glVertex2i(x, y);
  }
  glEnd();
}

void fillCircle(int cx, int cy, int r) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2i(cx, cy);  // Center of the circle
  int num_segs = 100;  // Number of segments for smoothness
  for (int i = 0; i <= num_segs; ++i) {
    float theta = 2.0f * 3.1415926f * (float)(i) / (float)(num_segs);
    int x = (int)(round(cx + r * cos(theta)));
    int y = (int)(round(cy + r * sin(theta)));
    glVertex2i(x, y);
  }
  glEnd();
}

void drawOval(int cx, int cy, int rx, int ry) {
  glBegin(GL_LINE_LOOP);
  int num_segs = 100;
  for (int i = 0; i < num_segs; ++i) {
    float theta = 2.0f * 3.1415926f * (float)(i) / (float)(num_segs);
    int x = (int)(round(cx + rx * cos(theta)));
    int y = (int)(round(cy + ry * sin(theta)));
    glVertex2i(x, y);
  }
  glEnd();
}

void fillOval(int cx, int cy, int rx, int ry) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2i(cx, cy);  // Center
  int num_segs = 100;
  for (int i = 0; i <= num_segs; ++i) {
    float theta = 2.0f * 3.1415926f * (float)(i) / (float)(num_segs);
    int x = (int)(round(cx + rx * cos(theta)));
    int y = (int)(round(cy + ry * sin(theta)));
    glVertex2i(x, y);
  }
  glEnd();
}

void drawPolygon(const int* points, int num_points) {
  if (num_points < 3) return; // Need at least 3 points to draw polygon outline
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < num_points; ++i) {
    glVertex2i(points[2*i], points[2*i + 1]);
  }
  glEnd();
}

void fillPolygon(const int* points, int num_points) {
  if (num_points < 3) return; // Need at least 3 points to fill polygon
  glBegin(GL_TRIANGLE_FAN);
  for (int i = 0; i < num_points; ++i) {
    glVertex2i(points[2*i], points[2*i + 1]);
  }
  glEnd();
}

GLuint loadTarga(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    TGAHeader header;
    fread(&header, sizeof(TGAHeader), 1, file);

    if (header.image_type != 2 && header.image_type != 10) { // Support uncompressed RGB and RGBA
        printf("Unsupported TGA format\n");
        fclose(file);
        return 0;
    }

    int width = header.width;
    int height = header.height;
    int channels = header.bits_per_pixel / 8;

    unsigned char* data = (unsigned char*)malloc(width * height * channels);
    fread(data, width * height * channels, 1, file);
    fclose(file);

    // Flip the image vertically
    for (int i = 0; i < height / 2; i++) {
        for (int j = 0; j < width * channels; j++) {
            unsigned char temp = data[i * width * channels + j];
            data[i * width * channels + j] = data[(height - 1 - i) * width * channels + j];
            data[(height - 1 - i) * width * channels + j] = temp;
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

    free(data); // Free the image memory
    return textureID;
}

void drawTarga(GLuint texture, int texWidth, int texHeight, float posX, float posY) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        // Map the full texture coordinates with correct alpha transparency
        glTexCoord2f(0.0f, 0.0f); glVertex2f(posX, posY);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(posX + texWidth, posY);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(posX + texWidth, posY + texHeight);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(posX, posY + texHeight);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

// Helper function to rotate a point (x, y) around pivot (pivotX, pivotY) by angle rotation
static void rotate_point(float x, float y, float pivotX, float pivotY,
                  float cosR, float sinR, float posX, float posY,
                  float* outX, float* outY) {
    float dx = x - pivotX;
    float dy = y - pivotY;
    *outX = cosR * dx - sinR * dy + posX + pivotX;
    *outY = sinR * dx + cosR * dy + posY + pivotY;
}

void drawTargaX(GLuint texture, int texWidth, int texHeight, Frame clip,
                float posX, float posY,
                MYGL2DFlip flipFlags, float rotation,
                float pivotX, float pivotY,
                float scale) {
    // Enable texture and blending for transparency
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Calculate texture coordinates (flip y for OpenGL)
    float u1 = (float)clip.x / (float)texWidth;
    float v1 = (float)(texHeight - (clip.y + clip.h)) / (float)texHeight;
    float u2 = (float)(clip.x + clip.w) / (float)texWidth;
    float v2 = (float)(texHeight - clip.y) / (float)texHeight;

    // Apply horizontal flip by swapping u coords
    if (flipFlags & MYGL2D_FLIP_H) {
        float temp = u1; u1 = u2; u2 = temp;
    }

    // Apply vertical flip by swapping v coords
    if (flipFlags & MYGL2D_FLIP_V) {
        float temp = v1; v1 = v2; v2 = temp;
    }

    // Precompute cos and sin of rotation angle
    float cosR = cosf(rotation);
    float sinR = sinf(rotation);

    // Define scaled quad corners relative to posX, posY
    float x0 = 0.0f,               y0 = 0.0f;
    float x1 = (float)clip.w * scale, y1 = 0.0f;
    float x2 = (float)clip.w * scale, y2 = (float)clip.h * scale;
    float x3 = 0.0f,               y3 = (float)clip.h * scale;

    // Rotate each point around pivot and translate to position
    float rx0, ry0, rx1, ry1, rx2, ry2, rx3, ry3;
    rotate_point(x0, y0, pivotX, pivotY, cosR, sinR, posX, posY, &rx0, &ry0);
    rotate_point(x1, y1, pivotX, pivotY, cosR, sinR, posX, posY, &rx1, &ry1);
    rotate_point(x2, y2, pivotX, pivotY, cosR, sinR, posX, posY, &rx2, &ry2);
    rotate_point(x3, y3, pivotX, pivotY, cosR, sinR, posX, posY, &rx3, &ry3);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw textured quad with texture coords and rotated vertices
    glBegin(GL_QUADS);
        glTexCoord2f(u1, v1); glVertex2f(rx0, ry0);
        glTexCoord2f(u2, v1); glVertex2f(rx1, ry1);
        glTexCoord2f(u2, v2); glVertex2f(rx2, ry2);
        glTexCoord2f(u1, v2); glVertex2f(rx3, ry3);
    glEnd();

    // Disable blending and texture
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void updateAnimation(Animation* animation, float deltaTime) {
    animation->elapsedTime += deltaTime;
    if (animation->elapsedTime >= animation->frameDuration) {
        animation->elapsedTime = 0;
        animation->currentFrame++;
        if (animation->currentFrame >= animation->frameCount)
            animation->currentFrame = 0; // loop animation
    }
}

