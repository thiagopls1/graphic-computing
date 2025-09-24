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

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
int n_triangles = 2; // Quantos triângulos serão criados

static const char* fShader = "./shaders/fragment_shader.glsl";
static const char* vShader = "./shaders/vertex_shader.glsl";

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

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj1);
}

void CreateShaders() {
    Shader* shader1 = new Shader();
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

    // Criar N Triângulos
    for (int i = 0; i < n_triangles; i++) {
        CreateObjects();
        CreateShaders();
    }

    // Projeção (Câmera)
    glm::mat4 projection = glm::perspective(
        45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f,
        100.0f);

    // Variáveis para rotacionar a pirâmide
    float rotationMax = 360.0f;
    float rotationIncrement = 1.0f;
    float rotationAngle = 0.0f;

    while (!mainWindow.getShouldClose()) {
        // Ativa inputs e eventos da window
        glfwPollEvents();

        if (rotationAngle < rotationMax) {
            rotationAngle += rotationIncrement;
        }
        else {
            rotationAngle = 0.0f;
        }

        /********************************
         * Cor de fundo da tela
         *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************************
         * Piramide
         *********************************/

        for (int i = 0; i < shaderList.size(); i++) {
            // Usar o programa
            shaderList[i].UseShader();
            // Movimentação da projeção da câmera
            glUniformMatrix4fv(shaderList[i].GetProjectionLocation(), 1, GL_FALSE,
                glm::value_ptr(projection));
        }

        // Piramide lado 1
        // cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
        glm::mat4 model_0(1.0f);
        // traduz o modelo para movimentar a posição (x, y, z)
        model_0 = glm::translate(model_0, glm::vec3(0.0f, 0.0f, -2.5f));
        model_0 = glm::scale(model_0, glm::vec3(0.4f, 0.4f, 1.0f));

        // Rotação lado 1
        model_0 = glm::rotate(model_0, glm::radians(90.0f + rotationAngle),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(shaderList[0].GetModelLocation(), 1, GL_FALSE,
            glm::value_ptr(model_0));
        meshList[0]->RenderMesh();

        // Piramide lado 2
        glm::mat4 model_1(1.0f);
        model_1 = glm::translate(model_1, glm::vec3(-0.0f, 0.0f, -2.5f));
        model_1 = glm::scale(model_1, glm::vec3(0.4f, 0.4f, 1.0f));

        // Rotação lado 2
        model_1 = glm::rotate(model_1, glm::radians(-90.0f + rotationAngle),
            glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(shaderList[1].GetModelLocation(), 1, GL_FALSE,
            glm::value_ptr(model_1));
        meshList[1]->RenderMesh();

        glUseProgram(0); // Removo o Programa da memória

        mainWindow.swapBuffers();
    }

    // Deleta os objetos criados
    mainWindow.terminate();
    return 0;
}