#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): m_Count(count)
{
    ASSERT( sizeof(unsigned int) == sizeof(GLuint))
    
    //for blocking buffer
    GLCall(glGenBuffers(1, &m_RendererID));

    /*
        for selecting buffer and use it as array
        (example :
                     photoshop if i select layer then i can't draw on another layer
                     similary we will give draw call for selected buffer only
        )
   */

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    //Specify size and give data
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}
