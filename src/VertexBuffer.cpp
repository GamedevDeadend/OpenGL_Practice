#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //for blocking buffer
    GLCall(glGenBuffers(1, &m_RendererID));

    /*
        for selecting buffer and use it as array
        (example :
                     photoshop if i select layer then i can't draw on another layer
                     similary we will give draw call for selected buffer only
        )
   */

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    //Specify size and give data
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}
