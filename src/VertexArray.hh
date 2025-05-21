#pragma once

/*
 * Notes for myself
 *
 * class VertexArray to deal with vertex array objects [Abstractions]
 * - glGenVertexArrays(howMany, &VAOId)
 * Constructor does above
 *
 * - glDeleteVertexArrays(howMany, &VAOId)
 * Destructor does above
 *
 * bind() calls glBindVertexArray(VAOId)
 * unbind() calls glBindVertexArray(0)
 *
 * addBuffer() makes use of VertexBufferLayout and stores the state of a VertexBuffer
 * binds the VAO and immediately binds the VBO
 * Then specifies the attribute layout of the VBO
 */

#include "VertexBuffer.hh"
#include "VertexBufferLayout.hh"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& VBO, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;

private:
    unsigned int m_rendererId;
};
