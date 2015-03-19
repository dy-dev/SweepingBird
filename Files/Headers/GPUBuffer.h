#include <glew/glew.h>



namespace SweepingBirds
{
  class ShaderProgram;

  /*
    This class represents an OpenGL buffer
    holding random data. It uses RAII paradigm.
    */
  class GPUBuffer
  {
  public:
    /// The target used by GPUBuffer
    static const GLenum BUFFER_TARGET;

    GPUBuffer();
    ~GPUBuffer();

    ///Set the buffer's name to be used in shaders
    void setName(const std::string& name);
    const std::string& getName() const;

    void bind() const;
    void unbind() const;

    GLsizeiptr getSize() const;

    void connectToShader(const ShaderProgram& shaderProgram) const;

    /**Initialize the data in the buffer
     * @see https://www.opengl.org/wiki/GLAPI/glBufferStorage
     */
    void setData(GLsizeiptr size, const GLvoid* data, GLbitfield flags);

    /**Updates the data inside the buffer starting at the beginning of it.
     * @warning You need: dataSize <= getSize()
     */
    void updateData(const GLvoid* data, GLsizeiptr dataSize);

  private:
    static GLuint s_uiLastBindingPoint;

    GLuint m_uiBufferId;
    GLuint m_uiBindingPoint;
    GLsizeiptr m_siSize;

    std::string m_sName;
  };

}