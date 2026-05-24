#include "framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer(int iWidth, int iHeight) 
    : m_iWidth(iWidth), m_iHeight(iHeight), m_nFBO(0), m_nColorTexture(0), m_nRBO(0) 
{
  glGenFramebuffers(1, &m_nFBO);
  bind();
  // create a color attachment texture
  glGenTextures(1, &m_nColorTexture);
  glBindTexture(GL_TEXTURE_2D, m_nColorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nColorTexture, 0);
  // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
  glGenRenderbuffers(1, &m_nRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, m_nRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_iWidth, m_iHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_nRBO); // now actually attach it
  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer failed to complete!" << std::endl;
  }
  unbind();
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &m_nFBO);
  glDeleteTextures(1, &m_nColorTexture);
  glDeleteRenderbuffers(1, &m_nRBO);
}

auto Framebuffer::bind() -> void {
  glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);
}

auto Framebuffer::unbind() -> void {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto Framebuffer::getColorTexture() const -> unsigned int {
  return m_nColorTexture;
}

auto Framebuffer::resize(int iWidth, int iHeight) -> void {
  if (iWidth <= 0 || iHeight <= 0) {
    return;
  }

  m_iHeight = iHeight;
  m_iWidth = iWidth;

  bind();

  glBindTexture(GL_TEXTURE_2D, m_nColorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glBindRenderbuffer(GL_RENDERBUFFER, m_nRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_iWidth, m_iHeight);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer resize failed to complete!" << std::endl;
  }
  
  unbind();
}