#include "VertexArray.hh"

VertexArray::VertexArray()
{
    glCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray()
{
    glCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::addBuffer(const VertexBuffer& VBO, const VertexBufferLayout& layout)
{
    bind();
    VBO.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(i, element.count_, element.type_,
                                     element.normalized_, layout.getStrid(),
                                     (const void*)offset));

        offset += element.count_ * VertexBufferElement::getSizeOfType(element.type_);
    }

}

void VertexArray::bind() const
{
    glCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unbind() const
{
    glCall(glBindVertexArray(0));
}
