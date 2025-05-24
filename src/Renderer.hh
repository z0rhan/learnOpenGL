#pragma once

#include "VertexArray.hh"
#include "IndexBuffer.hh"
#include "Shader.hh"
#include "error.hh"

#include <glad/glad.h>

class Renderer
{
public:
    void draw(const VertexArray& VAO, const IndexBuffer& EBO, const Shader& shader);
};