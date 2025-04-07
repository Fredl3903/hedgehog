#include "VBO.hpp"
#include "IBO.hpp"

class VAO {
    private:
        
        GLuint ID;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<IBO> ibo;

    public:
        VAO();
        virtual ~VAO();
        // move constructors
        VAO(VAO&& other) noexcept
            : ID(other.ID),
            vbo(std::move(other.vbo)),
            ibo(std::move(other.ibo)) {
            other.ID = 0; // Reset the moved-from object
        }
        VAO& operator=(VAO&& other) noexcept {
            if (this != &other) {
                ID = other.ID;
                vbo = std::move(other.vbo);
                ibo = std::move(other.ibo);
                other.ID = 0; // Reset the moved-from object
            }
            return *this;
        }
        // explicitly delete copying
        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;

        GLuint getID() { return ID; }

        void linkVBO(std::unique_ptr<VBO> VBO);
        void linkIBO(std::unique_ptr<IBO> IBO);

        void addAttrib(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);

        void bind();
        void unbind();

        bool isBound() const;
};




// ************************************************************************************************


VAO::VAO() {
    glGenVertexArrays(1, &ID);
}
VAO::~VAO() {
    glDeleteVertexArrays(1, &ID);
}
void VAO::bind() {
    glBindVertexArray(ID);
}
void VAO::unbind() {
    glBindVertexArray(0);
}
bool VAO::isBound() const {
    GLint currentBuffer = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBuffer);
    if (currentBuffer == static_cast<GLint>(ID)) {
        // std::cout << "_ARRAY_BUFFER bound with VAO: " << ID << std::endl;
        return true;
    } else {
        // std::cout << "VAO (" << ID << ") is not bound" << std::endl;
        return false;
    }
}

void VAO::linkVBO(std::unique_ptr<VBO> VBO) {
    vbo = std::move(VBO);
}
void VAO::linkIBO(std::unique_ptr<IBO> IBO) {
    ibo = std::move(IBO);
}
void VAO::addAttrib(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset) {
    bind();
    
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);

    unbind();
}