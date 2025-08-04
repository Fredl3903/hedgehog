#include <C:/OpenGL/GLFW//include/GLFW/glfw3.h>
#include <C:/OpenGL/GLEW/include/GL/glew.h>

#include <iostream>

class IBO {
    private:
        GLuint ID;

    public:
        IBO();
        ~IBO();

        void data(const GLuint* indexArray, int arraySize);
        void bind();
        void unbind();

        GLuint getID() { return ID; }

        bool isBound() const;
};




// ************************************************************************************************


IBO::IBO() {
    glGenBuffers(1, &ID);
}
IBO::~IBO() {
    glDeleteBuffers(1, &ID);
}
void IBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void IBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void IBO::data(const GLuint* indexArray, int arraySize) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraySize, indexArray, GL_STATIC_DRAW);
}
bool IBO::isBound() const {
    GLint currentBuffer = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer == static_cast<GLint>(ID)) {
        // std::cout << "GL_ELEMENT_ARRAY_BUFFER bound with IBO: " << ID << std::endl;
        return true;
    } else {
        // std::cout << "IBO (" << ID << ") is not bound" << std::endl;
        return false;
    }
}