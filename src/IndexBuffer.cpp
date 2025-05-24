#include "IndexBuffer.hh"
#include "error.hh"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glCall(glGenBuffers(1, &m_rendererId));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
                        GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
    glCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::unbind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

