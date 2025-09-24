#include "shader.h"

Shader::Shader() {
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode,
    const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation,
    const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStrem(fileLocation, std::ios::in);

    if (!fileStrem.is_open()) {
        printf("Erro ao tentar abrir o arquivo %s\n", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStrem.eof()) {
        std::getline(fileStrem, line);
        content.append(line + "\n");
    }

    fileStrem.close();
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    // 1. Criar um programa
    shaderID = glCreateProgram(); // Inicia o programa
    if (!shaderID) {
        printf("Erro ao criar o programa\n");
        return;
    }

    // 2. Compila o Vertex Shader
    AddShader(GL_VERTEX_SHADER, vertexCode);
    // 3. Compila o Fragment Shader
    AddShader(GL_FRAGMENT_SHADER, fragmentCode);

    // 4. Criar o link do programa
    glLinkProgram(shaderID); // Programa

    // 5. Validação de link do programa
    GLint result = 0;
    glGetProgramiv(
        shaderID, GL_LINK_STATUS,
        &result); // Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL,
            log); // Busca o texto caso de algum erro na compila��o
        printf("Erro ao linkar o programa: '%s'\n", log);
        return;
    }

    // 6. Validação do programa
    glValidateProgram(shaderID);
    result = 0;
    glGetProgramiv(
        shaderID, GL_VALIDATE_STATUS,
        &result); // Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL,
            log); // Busca o texto caso de algum erro na compilação
        printf("Erro ao validar o programa: '%s'\n", log);
        return;
    }

    uniformModel = glGetUniformLocation(
        shaderID, "model"); // Procura a entrada chamada model
    uniformProjection = glGetUniformLocation(
        shaderID, "projection"); // Procura a entrada chamada projection
}

void Shader::AddShader(GLenum shaderType, const char* shaderCode) {
    // Começamos a compilar cada shader
    // 1. Criar um shader
    GLuint shader = glCreateShader(shaderType);

    // 2. Atribui o código do GLSL para o shader
    // 2.1 Guarda e converte a variável char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;

    // 2.2 Anexa o código ao shader
    glShaderSource(shader, 1, code,
        NULL); // 1: número de códigos que estão sendo enviados | NULL:
    // final da string, termina o código

// 3. Compila o shader
    glCompileShader(shader); // compila o shader

    // 4. Tratamento de erros de compilação
    GLint result = 0;
    glGetShaderiv(
        shader, GL_COMPILE_STATUS,
        &result); // Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL,
            log); // Busca o texto caso de algum erro na compilação
        printf("Erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    // 5. Anexa o shader ao programa
    glAttachShader(shaderID, shader); // Anexa shader ao programa "variável
    // global"
}

void Shader::UseShader() { glUseProgram(shaderID); }

void Shader::ClearShader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader() { ClearShader(); }

GLuint Shader::GetProjectionLocation() { return uniformProjection; }

GLuint Shader::GetModelLocation() { return uniformModel; }
