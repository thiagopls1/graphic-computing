#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
// VAO � o ponteiro do programa que estou rodando, espa�o de mem�ria que representa aquele objeto
// VBO � o ponteiro para os meus dados brutos, s�o os dados do objeto a ser representado pelo VAO 
// shaderProgram � qual programa estou rodando 
// todo programa pode ser chamado de shader
GLuint VAO, VBO, shaderProgram;

// aqui estamos fazendo um programa (shader) em GLSL

// shader para renderizar pontos na tela
static const char* vertexShader = "                                                        \n\
#version 330                                                                               \n\
                                                                                           \n\
                                                                                           \n\
// passando um argumento para o inicio do programa (args do C//                            \n\
// estou passando um argumento de entrada na primeira posi��o                              \n\
// esse argumento deve ser um vetor de duas posi��es                                       \n\
layout(location=0) in vec2 pos;                                                            \n\
                                                                                           \n\
void main() {                                                                              \n\
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);                                            \n\
}                                                                                          \n\
";

// fragment pode ser lido como "a partir desse ponto o que fa�o com ele?"
// shader para atribuir cores aos pontos

static const char* fragmentShader = "                                                      \n\
#version 330                                                                               \n\
                                                                                           \n\
// diferente da entrada por layout, uniform � uma entrada em tempo de execu��o             \n\
uniform vec3 triColor;																       \n\
out vec4 color;																		       \n\
                                                                                           \n\
void main() {                                                                              \n\
	color = vec4(triColor, 1.0);                                                           \n\
}                                                                                          \n\
";

void create_triangle() {
	GLfloat vertices[] = { // nosso buffer de vert�ces
		0.0f, 1.0f, // vertice 1 
		-1.0f, -1.0f, // vertice 2
		1.0f, -1.0f // vertice 3
	};

	// iniciar um VAO
	// vamos abrir um espa�o de mem�ria na placa de v�deo
	// pegamos o endere�o virutal da placa de video e guardamos ele na vari�vel l� em cima
	glGenVertexArrays(1, &VAO);

	// estamos dizendo que a partir desse momento todas as altera��es ser�o feitas nesse espa�o de mem�ria
	// agora tudo o que vamos criar ser� nesse espa�o de mem�ria como os pontos da nossa tela
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO); // alocar um buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // a partir desse momento estamos modificando esse buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // explicando onde est�o os dados
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // explicando como interpretar esses dados
	glEnableVertexAttribArray(0);  // location

	// deixar de apontar para o VAO e VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// fun��o para executar um buffer na tela
void add_shader(GLuint program, const char* shaderCode, GLenum type) {
	GLuint _shader = glCreateShader(type);

	// converte char para glchar
	const GLchar* code[1];
	code[0] = shaderCode;
	// Compila��o dos shaders
	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);

	// tratar os erros

	glAttachShader(program, _shader);
}

void add_program() {
	// Cria��o do programa
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		cout << "Erro: falha ao criar o programa!";
	}

	add_shader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	add_shader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

	// Linking do programa com o shader compilado
	glLinkProgram(shaderProgram);
}

GLfloat random_float() {
	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_real_distribution<> distr(0.0, 1.0);
	GLfloat random_float = distr(rng);
	return random_float;
}

int main() {
	// GLFW Cria a nossa janela
	// inicando GLFW
	if (!glfwInit()) {
		cout << "Erro: GLFW n�o foi iniciado";
		glfwTerminate();
		return -1;
	}

	// set nas coisas obrigat�rias para subir uma janela
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // vers�o m�xima permitida
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // vers�o m�nima
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // somente as fun��es core
	// Setando retrocompatibilidade como true
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // qual a precis�o de ponto flutuante que vamos usar, precis�o da placa

	// cria��o de janela 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangulo", NULL, NULL);
	if (!window) {
		cout << "Erro: N�o foi poss�vel criar a janela";
		glfwTerminate();
		return -1;
	}

	// guardando o tamanho real da minha tela 
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(window); // tornando essa janela como principal

	//iniciando o Glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Erro: n�o foi poss�vel iniciar o glew";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	create_triangle();
	add_program();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat r = random_float();
		GLfloat g = random_float();
		GLfloat b = random_float();

		// Altera cor do triangulo
		GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniformColor, r, g, b);

		// Desenhando o triangulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
			// Triangulo, come�ando na pos 0, Numero de pontos 3
			glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}