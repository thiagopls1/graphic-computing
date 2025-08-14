#include <stdio.h>
#include <GL\glew.h>
#include <GLFW/glfw3.h>

const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

int main() {
    // GL -> GLEW
    // GLFW -> GLFW

    // GLFW Init
    printf("Iniciando GLFW...");
    if (!glfwInit()) {
        printf("GLFW não foi iniciado");
        glfwTerminate();
        return 1;
    }

    // Versão necessária do OpenGL para rodar
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core vai usar apenas OpenGL (Apenas a base)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // GL_TRUE pega o "true" da placa de vídeo

    // Criação da janela
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);

    if (!window) {
        printf("Erro ao criar a janela");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    // Indica ao GLFW qual é a janela padrão
    glfwMakeContextCurrent(window);

    // Permite o uso de features experimentais do glew, feitas pelas próprias empresas da placa de vídeo
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Erro ao iniciar o Glew");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Janela vai começar no ponto 0,0 do plano cartesiano
    // Passamos a altura e largura real (de buffer) para o viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    while (!glfwWindowShouldClose(window)) {
        // RGBA
        glClearColor(1.0f, 0.4f, 0.2f, 1.0f);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}