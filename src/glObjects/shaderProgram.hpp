#include "../util/getTxt.hpp"

GLuint compileShader(const char* source, GLenum shaderType) {
    GLint success;
    
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        glDeleteShader(shader);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}




class hhShaderProgram {
    private:
        // std::string vertSrc;
        // std::string fragSrc;
        GLuint ID;
    public:

        hhShaderProgram(const char* vertShaderSrcFile, const char* fragShaderSrcFile);
        void use();
        GLuint getID() {return ID;};
        void destroy();
        ~hhShaderProgram() {
            if (ID != 0) {
                glDeleteProgram(ID);
                ID = 0;
            }
        }
};

hhShaderProgram::hhShaderProgram(const char* vertShaderSrcFile, const char* fragShaderSrcFile) {

    std::string vertFilePath = "../src/shaders/" + std::string(vertShaderSrcFile);
    std::string fragFilePath = "../src/shaders/" + std::string(fragShaderSrcFile);

    std::string vertSrc = getFileContents(vertFilePath.c_str());
    std::string fragSrc = getFileContents(fragFilePath.c_str());
    
    GLuint vertShader = compileShader(vertSrc.c_str(), GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);

    if (vertShader == 0 || fragShader == 0) {
        std::cerr << "Failed to compile one or more shaders. Program creation aborted." << std::endl;
        return;
    }

    GLint success;
    GLchar infoLog[512];

    ID = glCreateProgram();
    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void hhShaderProgram::use() {
    glUseProgram(ID);
}

void hhShaderProgram::destroy() {
    if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
    }
}