#pragma once
#include "../stb_image/stb_image.h"
#include <GL/glew.h>

class Texture {
public:
  Texture();
  Texture(char *fileLoc);

  void loadTexture();
  void useTexture();
  void clearTexture();

  ~Texture();

private:
  GLuint textureID;
  int width, height, bitDepth;
  char *fileLocation;
};
