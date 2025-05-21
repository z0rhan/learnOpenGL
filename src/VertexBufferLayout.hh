#pragma once

/*
 * Notes for myself
 *
 * class VertexBufferElemen to deal with the layout of the vertices
 * makes use of VertexBufferElement to store the parameters required for
 * - glVertexAttribPointer(index, count_, type_, isNormalized_, stride, offset)
 *   stride is the jump for the same attribute
 *   offset is where each attribute starts
 */

#include "Renderer.hh"

#include <glad/glad.h>
#include <vector>

struct VertexBufferElement
{
    unsigned int type_;
    unsigned int count_;
    unsigned char normalized_;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() :m_stride(0) {};

    template <typename T>
    void push(unsigned int count);

    inline unsigned int getStrid() const {return m_stride;};
    inline const std::vector<VertexBufferElement> getElements() const {return m_elements;};
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};
