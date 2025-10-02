#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "mesh/mesh.h"
#include "shader/shader.h"
#include "texture/texture.h"
#include "window/window.h"

std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

static const char *fShader = "./shaders/fragment_shader.glsl";
static const char *vShader = "./shaders/vertex_shader.glsl";

// Old version of FPS
GLfloat deltaTime = 0.0f, lastime = 0.0f;

void CreateObjects() {
  std::vector<unsigned int> indices = {
      0, 1, 2, // Frente
      0, 1, 3, // Lateral Direita
      0, 2, 3, // Lateral Esquerda
      3, 1, 2  // Base
  };

  std::vector<GLfloat> vertices = {
      0.0f,  1.0f,  0.0f, 0.5f, 1.0f, // Vértice 0 (x,y,z, u,v)
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // Vértice 1 (x,y,z, u,v)
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Vértice 2 (x,y,z, u,v)
      0.0f,  -1.0f, 1.0f, 0.5f, 0.0f  // Vértice 3 (x,y,z, u,v)
  };

  Mesh *obj1 = new Mesh();
  obj1->CreateMesh(vertices.data(), indices.data(), vertices.size(),
                   indices.size());
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->CreateMesh(vertices.data(), indices.data(), vertices.size(),
                   indices.size());
  meshList.push_back(obj2);
}

void CreateFloor() {
  std::vector<GLfloat> vertices = {
      -10.0f, 0.0f, 10.0f,  5.0f,  10.0f, // Vértice 0 (x,y,z, u,v)
      -10.0f, 0.0f, -10.0f, 0.0f,  0.0f,  // Vértice 1 (x,y,z, u,v)
      10.0f,  0.0f, 10.0f,  10.0f, 5.0f,  // Vértice 2 (x,y,z, u,v)
      10.0f,  0.0f, -10.0f, 5.0f,  10.0f  // Vértice 3 (x,y,z, u,v)
  };

  std::vector<unsigned int> indices = {
      0, 1, 2, // Triângulo 1
      3, 1, 2  // Triângulo 2
  };

  Mesh *obj1 = new Mesh();
  obj1->CreateMesh(vertices.data(), indices.data(), vertices.size(),
                   indices.size());
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->CreateMesh(vertices.data(), indices.data(), vertices.size(),
                   indices.size());
  meshList.push_back(obj2);
}

void CreateShader() {
  Shader *shader1 = new Shader();
  shader1->CreateFromFile(vShader, fShader);
  shaderList.push_back(*shader1);
}

int main() {
  // Cria a window
  Window mainWindow(800, 600);
  if (mainWindow.initialize() != 0) {
    printf("Falha ao inicializar a janela!\n");
    return 1;
  }

  CreateObjects(); // Coloca os dados na memória da placa de vídeo
  CreateFloor();   // Cria o triangulo do chão
  CreateShader();  // Cria os shaders

  // Projeção (Câmera)
  Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 8.0f);

  // Carrega as Textures
  Texture brickTexture = Texture((char *)("textures/brick.png"));
  brickTexture.loadTexture();
  Texture dirtTexture = Texture((char *)("textures/dirt.png"));
  dirtTexture.loadTexture();

  glm::mat4 projection = glm::perspective(
      1.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f,
      100.0f);

  while (!mainWindow.getWindowShouldClose()) {
    // Old Version of FPS
    GLfloat now = glfwGetTime();
    deltaTime = now - lastime;
    lastime = now;

    // Ativa inputs e eventos da window
    glfwPollEvents();

    // Controle do teclado
    camera.keyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(),
                        deltaTime);

    /********************************
     * Cor de fundo da tela
     *********************************/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************************
     * Piramides
     *********************************/
    shaderList[0].UseProgram(); // Usar o programa
    glUniformMatrix4fv(
        shaderList[0].getUniformProjection(), 1, GL_FALSE,
        glm::value_ptr(projection)); // Movimenta��o da proje��o da camera
    glUniformMatrix4fv(shaderList[0].getUniformView(), 1, GL_FALSE,
                       glm::value_ptr(camera.calculateViewMatrix()));

    /********************************
     * Piramide 1
     *********************************/

    // cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
    glm::mat4 model(1.0f);

    // traduz o modelo para movimentar a posição (x,y,z)
    model = glm::translate(model, glm::vec3(0.0f, -0.245f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    brickTexture.useTexture();
    meshList[0]->RenderMesh();

    /********************************
     * Piramide 2
     *********************************/
    // Cria uma matriz 4x4 colocando 1.0f em cada uma das posições
    model = glm::mat4(1.0f);

    // traduz o modelo para movimentar a posição (x,y,z)
    model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[1]->RenderMesh();

    /********************************
     * Chão (Cima)
     *********************************/
    // Cria uma matriz 4x4 colocando 1.0f em cada uma das posições
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.65f, -2.5f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    /********************************
     * Chão (Fundo)
     *********************************/
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -10.65f, -12.5f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    /********************************
     * Chão (Frente)
     *********************************/
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -10.65f, 7.5f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    /********************************
     * Chão (Direita)
     *********************************/
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(10.0f, -10.65f, -2.5f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    /********************************
     * Chão (Direita)
     *********************************/
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, -10.65f, -2.5f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    /********************************
     * Chão (Baixo)
     *********************************/
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -20.65f, -2.5f));
    glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE,
                       glm::value_ptr(model));
    dirtTexture.useTexture();
    meshList[2]->RenderMesh();

    glUseProgram(0); // Removo o Programa da memória

    mainWindow.swapBuffers();
  }

  return 0;
}
