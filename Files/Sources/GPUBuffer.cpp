#include "GPUBuffer.h"
#include <ShaderProgram.h>

#include <string>
#include <cassert>

//For implementation see http://www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-uniform-buffers-objects/

using namespace SweepingBirds;

const GLenum GPUBuffer::BUFFER_TARGET = GL_UNIFORM_BUFFER;

GLuint GPUBuffer::s_uiLastBindingPoint = 0;

GPUBuffer::GPUBuffer()
{
  GLint nbMaxBuffers = 0;
  glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &nbMaxBuffers);
  assert(s_uiLastBindingPoint >= nbMaxBuffers - 1 && "Max number of buffers already allocated");

  glGenBuffers(1, &m_uiBufferId);

  if (s_uiLastBindingPoint != 0)
    m_uiBindingPoint = ++s_uiLastBindingPoint;
  else
    m_uiBindingPoint = s_uiLastBindingPoint++;
}

GPUBuffer::~GPUBuffer()
{
  glDeleteBuffers(1, &m_uiBufferId);
}

void GPUBuffer::setName(const std::string& name)
{
  m_sName = name;
}

const std::string& GPUBuffer::getName() const
{
  return m_sName;
}

void GPUBuffer::bind() const
{
  glBindBufferBase(BUFFER_TARGET, m_uiBindingPoint, m_uiBufferId);
}

void GPUBuffer::unbind() const
{
  glBindBuffer(BUFFER_TARGET, 0);
}

GLsizeiptr GPUBuffer::getSize() const
{
  return m_siSize;
}

void GPUBuffer::connectToShader(const ShaderProgram& shaderProgram) const
{
  GLuint programId = shaderProgram.get_program();
  GLuint blockIndex = glGetUniformBlockIndex(programId, m_sName.c_str());

  bind();
  glUniformBlockBinding(programId, blockIndex, m_uiBindingPoint);
  unbind();
}

void GPUBuffer::setData(GLsizeiptr size, const GLvoid* data, GLbitfield flags)
{
  bind();
  glBufferStorage(BUFFER_TARGET, size, data, flags);
  unbind();
}


void GPUBuffer::updateData(const GLvoid* data, GLsizeiptr dataSize)
{
  assert(dataSize <= m_siSize);

  bind();
  GLvoid* p = glMapBuffer(BUFFER_TARGET, GL_WRITE_ONLY);
  std::memcpy(p, data, dataSize);
  glUnmapBuffer(BUFFER_TARGET);
  unbind();
}
