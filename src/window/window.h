#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class Window {
public:
  Window();
  Window(GLint windowWidth, GLint windowHeight);
  ~Window();
  int initialize();
  void swapBuffers();

  GLfloat getBufferWidth() { return (GLfloat)bufferWidth; }
  GLfloat getBufferHeight() { return (GLfloat)bufferHeight; }
  bool getWindowShouldClose() { return glfwWindowShouldClose(mainWindow); }
  bool *getKeys() { return keys; }
  GLfloat getXChange();
  GLfloat getYChange();

private:
  GLFWwindow *mainWindow;
  GLint width, height;
  GLint bufferWidth, bufferHeight;

  // Configuração de mouse e teclado
  void createCallbacks();

  // Configuração para teclado
  bool keys[1024];
  static void handleKeys(GLFWwindow *window, int key, int code, int action,
                         int mode);

  // Configuração para mouse
  GLfloat lastX;       // Ultima posição de X
  GLfloat lastY;       // Ultima posição de Y
  GLfloat xChange;     // Alteração de X
  GLfloat yChange;     // Alteração de Y
  bool mouseFirstMove; // Verifica se é o primeiro movimento do mouse
  static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};
;
