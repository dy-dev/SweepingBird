#pragma once

#include <glew/glew.h>

namespace SweepingBirds
{

  /*
    This class represents an OpenGL buffer
    holding random data. It uses RAII paradigm.
    */
  class GPUBuffer
  {
  public:
    /// The target used by GPUBuffer
    static const GLenum BUFFER_TARGET;

    ///For format see: https://www.opengl.org/sdk/docs/man/html/glTexBuffer.xhtml
    GPUBuffer(GLenum format);
    ~GPUBuffer();

    /**Activate the buffer and binds it to
     * the corresponding texture unit.
     * @param textureUnit: the texture unit to bind the buffer to.
    */
    void activate(GLenum textureUnit) const;
    void deactivate() const;

    GLsizeiptr getSize() const;

    /**Initialize the data in the buffer
     * @see https://www.opengl.org/wiki/GLAPI/glBufferStorage
     */
    void setData(GLsizeiptr size, const GLvoid* data);

    /**Updates the data inside the buffer starting at the beginning of it.
     * @warning You need: dataSize <= getSize()
     */
    void updateData(const GLvoid* data, GLintptr offset, GLsizeiptr dataSize);

  private:
    void bind() const;
    void unbind() const;

    GLenum m_eFormat;
    GLuint m_uiBufferId;
    GLsizeiptr m_siSize;

  };

}