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




// Function to draw TGA image
void drawTarga(GLuint textureID, int x, int y) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Calculate the width and height of the texture
    GLint textureWidth, textureHeight;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
    
    // Draw the texture as a quad at the specified pixel position
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y); // Bottom left
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + textureWidth, y); // Bottom right
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + textureWidth, y + textureHeight); // Top right
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + textureHeight); // Top left
    glEnd();
    
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

void drawTargaWithClip(GLuint texture, int texWidth, int texHeight, Frame clip, float posX, float posY) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float u1 = (float)clip.x / (float)texWidth;
    float v1 = (float)(texHeight - (clip.y + clip.h)) / (float)texHeight;  // Flip y to OpenGL texture coord
    float u2 = (float)(clip.x + clip.w) / (float)texWidth;
    float v2 = (float)(texHeight - clip.y) / (float)texHeight;

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        glTexCoord2f(u1, v1); glVertex2f(posX, posY);
        glTexCoord2f(u2, v1); glVertex2f(posX + clip.w, posY);
        glTexCoord2f(u2, v2); glVertex2f(posX + clip.w, posY + clip.h);
        glTexCoord2f(u1, v2); glVertex2f(posX, posY + clip.h);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

