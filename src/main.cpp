#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh/mesh.h"
#include "shader/shader.h"
#include "window/window.h"

std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

static const char *fShader = "./shaders/fragment_shader.glsl";
static const char *vShader = "./shaders/vertex_shader.glsl";

void CreateObjects() {
  unsigned int indices[] = {
      0, 1, 2, // Frente
      0, 1, 3, // Lateral Direita
      0, 2, 3, // Lateral Esquerda
      3, 1, 2  // Base
  };

  GLfloat vertices[] = {
      0.0f,  1.0f,  0.0f, // Vertice 0 (x,y,z)
      1.0f,  -1.0f, 0.0f, // Vertice 1 (x,y,z)
      -1.0f, -1.0f, 0.0f, // Vertice 2 (x,y,z)
      0.0f,  -1.0f, 1.0f  // Vertice 3 (x,y,z)
  };

  Mesh *obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
  meshList.push_back(obj1);
}

void CreateShaders() {
  Shader *shader1 = new Shader();
  shader1->CreateFromFiles(vShader, fShader);
  shaderList.push_back(*shader1);
}

int main() {
  // Cria a window
  Window mainWindow(800, 600);
  if (mainWindow.initialize() != 0) {
    printf("Falha ao inicializar a janela!\n");
    return 1;
  }

  // Criar o Triangulo
  CreateObjects();
  CreateShaders();

  glm::mat4 projection = glm::perspective(
      45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f,
      100.0f);

  while (!mainWindow.getShouldClose()) {
    // Ativa inputs e eventos da window
    glfwPollEvents();

    /********************************
     * Cor de fundo da tela
     *********************************/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************************
     * Piramide
     *********************************/
    shaderList[0].UseShader(); // Usar o programa

    // Movimenta��o da proje��o da camera
    glUniformMatrix4fv(shaderList[0].GetProjectionLocation(), 1, GL_FALSE,
                       glm::value_ptr(projection));

    // Piramide 1
    glm::mat4 model(1.0f); // cria uma matriz 4x4 e coloca os valores 1.0f em
                           // todas as posições
    model = glm::translate(
        model,
        glm::vec3(
            0.0, 0.0f,
            -2.5f)); // traduz o modelo para movimentar a posição (x, y, z)
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    // model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f,
    // 0.0f));
    glUniformMatrix4fv(shaderList[0].GetModelLocation(), 1, GL_FALSE,
                       glm::value_ptr(model));
    meshList[0]->RenderMesh();

    glUseProgram(0); // Removo o Programa da memória

    mainWindow.swapBuffers();
  }

  // Deleta os objetos criados
  mainWindow.terminate();
  return 0;
}
