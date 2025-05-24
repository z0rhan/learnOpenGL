#pragma once

#include <glad/glad.h>

/*
 * Notes for myself
 *
 * class to deal with element buffer objects [Abstractions]
 *
 * - glGenBuffers(howMany, &bufferId)
 * - glBindBuffers(bufferTarget[GL_ELEMENT_ARRAY_BUFFER,...], bufferId)
 * - glBufferData(bufferTarget, sizeOfData, Data, usage[GL_STATIC_DRAW,...])
 * The constructor deals with the above calls and stores the buffer as m_rendererId
 *
 * - glDeleteBuffers(howMany, &bufferId)
 * The destructor does the above
 *
 * bind() calls glBindBuffers(bufferTarget[GL_ELEMENT_ARRAY_BUFFER,...], bufferId)
 * unbind() calls glBindBuffers(bufferTarget[GL_ELEMENT_ARRAY_BUFFER,...], 0)
 */

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const {return m_count;}
private:
    unsigned int m_rendererId;
    unsigned int m_count;
};
