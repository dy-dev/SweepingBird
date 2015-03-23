#include "GPUBuffer.h"

#include <string>
#include <cassert>
#include <UtilityToolKit.h>

using namespace SweepingBirds;

const GLenum GPUBuffer::BUFFER_TARGET = GL_TEXTURE_BUFFER;

GPUBuffer::GPUBuffer(GLenum format)
	: m_eFormat(format),
	m_siSize(0)
{
	glGenBuffers(1, &m_uiBufferId);
}

GPUBuffer::~GPUBuffer()
{
	glDeleteBuffers(1, &m_uiBufferId);
}

void GPUBuffer::activate(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	bind();
	glTexBuffer(BUFFER_TARGET, m_eFormat, m_uiBufferId);
	UtilityToolKit::check_errors("GPUBuffer::activate");
}

void GPUBuffer::deactivate() const
{
	unbind();
}

GLsizeiptr GPUBuffer::getSize() const
{
	return m_siSize;
}

void GPUBuffer::setData(GLsizeiptr size, const GLvoid* data)
{
	bind();
	glBufferData(BUFFER_TARGET, size, data, GL_DYNAMIC_DRAW);
	unbind();
	assert(UtilityToolKit::check_errors("GPUBuffer::setData"));
	m_siSize = size;
}

void GPUBuffer::updateData(const GLvoid* data, GLintptr offset, GLsizeiptr dataSize)
{
	assert((offset + dataSize) <= m_siSize);

	bind();
	glBufferSubData(BUFFER_TARGET, offset, dataSize, data);
	unbind();
	assert(UtilityToolKit::check_errors("GPUBuffer::updateData"));
}


void GPUBuffer::bind() const
{
	glBindBuffer(BUFFER_TARGET, m_uiBufferId);
}

void GPUBuffer::unbind() const
{
	glBindBuffer(BUFFER_TARGET, 0);
}