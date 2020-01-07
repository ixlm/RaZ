#pragma once

#ifndef RAZ_RENDERSYSTEM_HPP
#define RAZ_RENDERSYSTEM_HPP

#include "RaZ/Entity.hpp"
#include "RaZ/Math/mathdef.h"
// #include "RaZ/Math/Matrix.hpp"
// #include "RaZ/Math/Vector.hpp"
#include "RaZ/Render/Cubemap.hpp"
#include "RaZ/Render/Framebuffer.hpp"
#include "RaZ/Render/RenderPass.hpp"
#include "RaZ/Render/UniformBuffer.hpp"
#include "RaZ/System.hpp"
#include "RaZ/Utils/Window.hpp"

namespace Raz
{

/// RenderSystem class, handling the rendering part.
class RenderSystem : public System, public WindowObserver
{
public:
  /// Creates a render system, initializing its inner data.
  RenderSystem();
  /// Creates a render system with a given scene size.
  /// \param sceneWidth Width of the scene.
  /// \param sceneHeight Height of the scene.
  RenderSystem(unsigned int sceneWidth, unsigned int sceneHeight);
  /// Creates a render system along with a Window.
  /// \param sceneWidth Width of the scene.
  /// \param sceneHeight Height of the scene.
  /// \param windowTitle Title of the window.
  /// \param antiAliasingSampleCount Number of anti-aliasing samples.
  RenderSystem(unsigned int sceneWidth, unsigned int sceneHeight,
               const std::string &windowTitle,
               uint8_t antiAliasingSampleCount = 1);

  bool hasWindow() const;
  const Window &getWindow() const;
  Window &getWindow();
  const Entity &getCameraEntity() const;
  Entity &getCameraEntity();
  const Camera &getCamera() const;
  Camera &getCamera();
  const GeometryPass &getGeometryPass() const;
  GeometryPass &getGeometryPass();
  const SSRPass &getSSRPass() const;
  SSRPass &getSSRPass();
  const Cubemap &getCubemap() const;

  void setCubemap(CubemapPtr cubemap);

  void resizeViewport(unsigned int width, unsigned int height);
  void createWindow(unsigned int width, unsigned int height,
                    const std::string &title = "");
  void enableGeometryPass(VertexShader vertShader, FragmentShader fragShader);
  void enableSSRPass(FragmentShader fragShader);
  void disableGeometryPass();
  void disableSSRPass();
  void linkEntity(const EntityPtr &entity) override;
  bool update(float deltaTime) override;
  void sendViewMatrix(const Mat4f &viewMat) const;
  void sendInverseViewMatrix(const Mat4f &invViewMat) const;
  void sendProjectionMatrix(const Mat4f &projMat) const;
  void sendInverseProjectionMatrix(const Mat4f &invProjMat) const;
  void sendViewProjectionMatrix(const Mat4f &viewProjMat) const;
  void sendCameraPosition(const Vec3f &cameraPos) const;
  void sendCameraMatrices(const Mat4f &viewProjMat) const;
  void sendCameraMatrices() const;
  void updateLight(const Entity *entity, std::size_t lightIndex) const;
  void updateLights() const;
  void removeCubemap(); 
  void updateShaders() const;
  void saveToImage(const std::string &fileName,
                   TextureFormat format = TextureFormat::RGB) const;
  void destroy() override;
  //--------------------WindowObserver begin --------------------
  virtual void onFramebufResize(int32_t w, int32_t h) ;
  //--------------------WindowObserver end--------------------

private:
  void initialize();
  void initialize(unsigned int sceneWidth, unsigned int sceneHeight);

  unsigned int m_sceneWidth{};
  unsigned int m_sceneHeight{};

  WindowPtr m_window{};
  Entity m_cameraEntity = Entity(0); //有一个默认的摄像机

  std::array<RenderPassPtr,
             static_cast<std::size_t>(RenderPassType::RENDER_PASS_COUNT)>
      m_renderPasses{};
  UniformBuffer m_cameraUbo =
      UniformBuffer(sizeof(Mat4f) * 5 + sizeof(Vec4f), 0);

  CubemapPtr m_cubemap{};
};

} // namespace Raz

#endif // RAZ_RENDERSYSTEM_HPP
