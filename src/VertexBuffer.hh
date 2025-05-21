#pragma once

/*
 * Notes for myself
 *
 * class VertexBuffer for dealing with what the name says [Abstractions]
 *
 * - glGenBuffers(howMany, &bufferId)
 * - glBindBuffers(bufferTarget[GL_ARRAY_BUFFER,...], bufferId)
 * - glBufferData(bufferTarget, sizeOfData, Data, usage[GL_STATIC_DRAW,...])
 * The constructor deals with the above calls and stores the buffer as m_rendererId
 *
 * - glDeleteBuffers(howMany, &bufferId)
 * The destructor does the above
 *
 * bind() calls glBindBuffers(bufferTarget[GL_ARRAY_BUFFER,...], bufferId)
 * unbind() calls glBindBuffers(bufferTarget[GL_ARRAY_BUFFER,...], 0)
 */

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size); // size is bytes
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    unsigned int m_rendererId;
};
