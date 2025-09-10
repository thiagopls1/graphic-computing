#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Roda na CPU
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
// VAO � o ponteiro do programa que estou rodando, espa�o de mem�ria que representa aquele objeto
// VBO � o ponteiro para os meus dados brutos, s�o os dados do objeto a ser representado pelo VAO 
// shaderProgram � qual programa estou rodando 
// todo programa pode ser chamado de shader
GLuint VAO, VBO, IBO, shaderProgram;

bool 
	direction = false, 
	directionSize = false;
float
	triOffset = 0.2f,
	triOffsetMax = 1.2f,
	triOffsetMin = 0.2f,
	triOffsetIncrement = 0.01f;
float
	triOffsetSize = 0.2f,
	triOffsetSizeMax = 1.2f,
	triOffsetSizeMin = 0.2f,
	triOffsetSizeIncrement = 0.01f;
float
	triCurrentAngle = 0.0f,
	triAngleIncrement = 1.0f;

float toRadians = 3.1415 / 180;

// aqui estamos fazendo um programa (shader) em GLSL
// shader para renderizar pontos na tela
static const char* vertexShader = "                                                        \n\
#version 330                                                                               \n\
                                                                                           \n\
// passando um argumento para o inicio do programa (args do C)                             \n\
// estou passando um argumento de entrada na primeira posi��o                              \n\
// esse argumento deve ser um vetor de duas posi��es                                       \n\
layout(location=0) in vec3 pos;                                                            \n\
uniform mat4 model;																		   \n\
uniform mat4 projection;			                                                       \n\
out vec4 vCol;  				                                                           \n\
                                                                                           \n\
void main() {                                                                              \n\
	gl_Position = projection * model * vec4(pos, 1.0);                                     \n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                                             \n\
}                                                                                          \n\
";

// fragment pode ser lido como "a partir desse ponto o que fa�o com ele?"
// shader para atribuir cores aos pontos

static const char* fragmentShader = "                                                      \n\
#version 330                                                                               \n\
                                                                                           \n\
in vec4 vCol;																	           \n\
// diferente da entrada por layout, uniform � uma entrada em tempo de execu��o             \n\
uniform vec3 triColor;																       \n\
out vec4 color;																		       \n\
                                                                                           \n\
void main() {                                                                              \n\
	color = vCol;																		   \n\
}                                                                                          \n\
";

void create_triangle() {
	// IBO -> Index Buffer Object
	unsigned int indices[] = {
		0, 1, 2, // BASE
		0, 1, 3, // Esquerda
		0, 2, 3, // Direita
		1, 3, 2, // Frente
	};

	GLfloat vertices[] = { // nosso buffer de vert�ces
		0.0f, 1.0f, 0.0f,	//	vertice 1 
		-1.0f, -1.0f, 0.0f, //  vertice 2
		1.0f, -1.0f, 0.0f,	//	vertice 3
		0.0f, 0.0f, 1.0f	//	vertice 4
	};

	// iniciar um VAO
	// vamos abrir um espa�o de mem�ria na placa de v�deo
	// pegamos o endere�o virutal da placa de video e guardamos ele na vari�vel l� em cima
	glGenVertexArrays(1, &VAO);

	// estamos dizendo que a partir desse momento todas as altera��es ser�o feitas nesse espa�o de mem�ria
	// agora tudo o que vamos criar ser� nesse espa�o de mem�ria como os pontos da nossa tela
	glBindVertexArray(VAO);

	// Gera o IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glGenBuffers(1, &VBO); // alocar um buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // a partir desse momento estamos modificando esse buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // explicando onde est�o os dados

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // explicando como interpretar esses dados
	glEnableVertexAttribArray(0);  // location

	// deixar de apontar para o VAO e VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat r = 1.0f;
		GLfloat g = 0.6f;
		GLfloat b = 0.8f;
		/*GLfloat r = random_float();
		GLfloat g = random_float();
		GLfloat b = random_float();*/

		// Altera cor do triangulo
		GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniformColor, r, g, b);

		// Muda o tamanho
		if (!direction) {
			triOffset += triOffsetIncrement;
		}
		else {
			triOffset -= triOffsetIncrement;
		}

		if (triOffset > triOffsetMax || triOffset < triOffsetMin) {
			direction = !direction;
		}

		triCurrentAngle += triAngleIncrement;
		if (triCurrentAngle >= 360) {
			triCurrentAngle = 0;
		}

		if (!directionSize) {
			triOffsetSize += triOffsetSizeIncrement;
		}
		else {
			triOffsetSize -= triOffsetSizeIncrement;
		}
		if (triOffsetSize > triOffsetSizeMax || triOffsetSize < triOffsetSizeMin) {
			directionSize = !directionSize;
		}

		GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
		GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");
		glm::mat4 model(1.0f);

		//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(triOffsetSize, triOffsetSize, triOffsetSize));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, triCurrentAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Desenhando o triangulo
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
			// Triangulo, come�ando na pos 0, Numero de pontos 3
			// glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				// Para cada face, temos 3 pontos
				glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}