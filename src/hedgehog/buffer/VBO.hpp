#ifndef HH_VBO
#define HH_VBO

class VBO {
    private:
        GLuint ID;

    public:
        VBO();
        ~VBO();

        void data(const GLfloat* vertexArray, int arraySize);
        void bind();
        void unbind();

        GLuint getID() { return ID; }

        bool isBound() const;
};




// ************************************************************************************************


VBO::VBO() {
    glGenBuffers(1, &ID);
}
VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}
void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::data(const GLfloat* vertexArray, int arraySize) {
  
    // for (int i = 0; i < 4; i++) {
    //     if (i < 3) {
    //       std::cout << vertexArray[3 * i] << ", "
    //         << vertexArray[3 * i + 1] << ", "
    //         << vertexArray[3 * i + 2] << std::endl;
    //     } else {
    //       std::cout << vertexArray[3 * i] << ", "
    //         << vertexArray[3 * i + 1] << std::endl;
    //     }
    // }

    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);
}
bool VBO::isBound() const {
    GLint currentBuffer = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer == static_cast<GLint>(ID)) {
        // std::cout << "GL_ARRAY_BUFFER bound with VBO: " << ID << std::endl;
        return true;
    } else {
        // std::cout << "VBO (" << ID << ") is not bound" << std::endl;
        return false;
    }
}

#endif