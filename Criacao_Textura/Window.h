#pragma once
#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();
	int initialize();
	void swapBuffers();

	GLfloat getBufferWidth() { return (GLfloat)bufferWidth; }
	GLfloat getBufferHeight() { return (GLfloat)bufferHeight; }
	bool getWindowShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Configura��o de mouse e teclado
	void createCallbacks();

	//Configura��o para teclado
	bool keys[1024];
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	//Configura��o para mouse
	GLfloat lastX; //ultima posi��o de X
	GLfloat lastY; //ultima posi��o de Y
	GLfloat xChange; //altera��o de X
	GLfloat yChange; //altera��o de Y
	bool mouseFirstMove; //verifica se � o primeiro movimento do mouse
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

