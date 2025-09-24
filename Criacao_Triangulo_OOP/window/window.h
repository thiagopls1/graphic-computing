#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class Window {
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int initialize();
	void terminate();
	~Window();

	GLfloat getBufferWidth() { return (GLfloat)bufferWidth; };
	GLfloat getBufferHeight() { return (GLfloat)bufferHeight; };

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); };

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};
