#include "RaZ/Render/Mesh.hpp"

namespace Raz {

void VertexArray::bind() const {
  glBindVertexArray(m_index);
  m_ebo.bind();
}

void VertexArray::unbind() const {
  glBindVertexArray(0);
  m_ebo.unbind();
}

void Mesh::load() const {
  m_vbo.bind();
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(m_vbo.getVertices().front()) * m_vbo.getVertices().size(),
               m_vbo.getVertices().data(),
               GL_STATIC_DRAW);
  m_vbo.unbind();

  m_vao.bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(getEbo().getVerticesIndices().front()) * getEbo().getVerticesIndices().size(),
               m_vao.getEbo().getVerticesIndices().data(),
               GL_STATIC_DRAW);

  m_vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  m_vao.unbind();

  /*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  texture.bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), , 0, GL_RGB, GL_UNSIGNED_BYTE, img.getData().data());*/
}

void Mesh::draw() const {
  //m_texture.bind();
  m_vao.bind();

  glDrawElements(GL_TRIANGLES, getFaceCount(), GL_UNSIGNED_INT, nullptr);
}

} // namespace Raz
