#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	//Vertex Array Object: VAO
	glGenVertexArrays(1, &VAO); //Gera um VAO ID
	glBindVertexArray(VAO); //Atribui o ID ao VAO

	glGenBuffers(1, &IBO); //Gera um IBO ID
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //Atribui o ID ao IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); //Atribuindo os valores dos indices para o IBO

	//Carrega os dados do v�rtice para a mem�ria da placa de v�deo
	//Vertex Buffer Object: VBO
	glGenBuffers(1, &VBO); //Gera um VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Atribui o ID ao VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); //Atribuindo os valores do vertice para o VBO
	//GL_STATIC_DRAW: Utilizado com desenhos que s�o fixos
	//GL_DYNAMIC_DRAW: Utilizado com desenhos que podem ter altera��es durtante a apresenta��o
	//GL_STREAM_DRAW: Desenho � utilizado somente 1 frame

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z) 
	//GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
	//0: offset
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));

	glEnableVertexAttribArray(0); //0: shader location
	glEnableVertexAttribArray(1); //1: texture location

	glBindBuffer(GL_ARRAY_BUFFER, 0); //remove o VBO da mem�ria
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //remove o IBO da mem�ria
	glBindVertexArray(0); //remove o VAO da mem�ria
}

void Mesh::RenderMesh() {
	glBindVertexArray(VAO); //Deixa na mem�ria os links para serem utilizados
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //Para garantir que placas de v�deo antigas que n�o trabalham com Indices ir�o receber os indices
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Remove o IBO da mem�ria
	glBindVertexArray(0); //Removo o VAO da mem�ria
}

Mesh::~Mesh() {
	if (VAO != 0) {
		glDeleteBuffers(1, &VAO);
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
	}
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
	}
	indexCount = 0;
}