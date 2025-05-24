#include "Renderer.hh"

void Renderer::draw(const VertexArray& VAO, const IndexBuffer& EBO, const Shader& shader)
{
    VAO.bind();
    EBO.bind();
    shader.bind();

    glCall(glDrawElements(GL_TRIANGLES, EBO.getCount(), GL_UNSIGNED_INT, nullptr));
}
