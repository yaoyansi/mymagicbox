#pragma once



// void initFrameBufferDepthBuffer(void);
// 
// void initFrameBufferTexture(void);
// 
// void initFrameBuffer(void);

void init(void);

//void keyOperations (void);

void renderTeapotScene(const double camRotateX, const double camRotateY);

void display (const double camRotateX, const double camRotateY);

void reshape (int width, int height);

//void keyPressed (unsigned char key, int x, int y);
//void keyUp (unsigned char key, int x, int y);

extern unsigned int fbo; // The frame buffer object
extern unsigned int fbo_depth; // The depth buffer for the frame buffer object
extern unsigned int fbo_texture; // The texture object to write our frame buffer object to
