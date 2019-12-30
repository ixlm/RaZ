#include "RaZ/Render/RenderSystem.hpp"
#include "RaZ/Math/Transform.hpp"
#include "RaZ/Render/Camera.hpp"
#include "RaZ/Render/Light.hpp"
#include "RaZ/Render/Mesh.hpp"
#include "RaZ/Render/Renderer.hpp"

namespace Raz
{

RenderSystem::RenderSystem() { initialize(); }
/// Creates a render system with a given scene size.
/// \param sceneWidth Width of the scene.
/// \param sceneHeight Height of the scene.
RenderSystem::RenderSystem(unsigned int sceneWidth, unsigned int sceneHeight)
    : RenderSystem()
{
  resizeViewport(sceneWidth, sceneHeight);
}
/// Creates a render system along with a Window.
/// \param sceneWidth Width of the scene.
/// \param sceneHeight Height of the scene.
/// \param windowTitle Title of the window.
/// \param antiAliasingSampleCount Number of anti-aliasing samples.
RenderSystem::RenderSystem(unsigned int sceneWidth, unsigned int sceneHeight,
                           const std::string &windowTitle,
                           uint8_t antiAliasingSampleCount)
    : m_window{Window::create(sceneWidth, sceneHeight, windowTitle,
                              antiAliasingSampleCount)}
{
  if (m_window)
  {
    m_window->registerObserver(this);
  }
  initialize(sceneWidth, sceneHeight);
}

bool RenderSystem::hasWindow() const { return (m_window != nullptr); }
const Window &RenderSystem::getWindow() const
{
  assert("Error: Window must be set before being accessed." && m_window);
  return *m_window;
}
Window &RenderSystem::getWindow()
{
  return const_cast<Window &>(
      static_cast<const RenderSystem *>(this)->getWindow());
}
const Entity &RenderSystem::getCameraEntity() const 
{ 
  return m_cameraEntity; 
}
Entity &RenderSystem::getCameraEntity() 
{ 
  return m_cameraEntity; 
}

const Camera &RenderSystem::getCamera() const
{
  return m_cameraEntity.getComponent<Camera>();
}

Camera &RenderSystem::getCamera()
{
  return m_cameraEntity.getComponent<Camera>();
}
GeometryPass &RenderSystem::getGeometryPass()
{
  return const_cast<GeometryPass &>(
      static_cast<const RenderSystem *>(this)->getGeometryPass());
}
SSRPass &RenderSystem::getSSRPass()
{
  return const_cast<SSRPass &>(
      static_cast<const RenderSystem *>(this)->getSSRPass());
}
const Cubemap &RenderSystem::getCubemap() const
{
  assert("Error: Cubemap must be set before being accessed." && m_cubemap);
  return *m_cubemap;
}

void RenderSystem::setCubemap(CubemapPtr cubemap)
{
  m_cubemap = std::move(cubemap);
}

const GeometryPass &RenderSystem::getGeometryPass() const
{
  const RenderPassPtr &renderPass =
      m_renderPasses[static_cast<std::size_t>(RenderPassType::GEOMETRY)];

  assert("Error: Geometry pass isn't enabled." && renderPass);

  return static_cast<const GeometryPass &>(*renderPass);
}

const SSRPass &RenderSystem::getSSRPass() const
{
  const RenderPassPtr &renderPass =
      m_renderPasses[static_cast<std::size_t>(RenderPassType::SSR)];

  assert("Error: SSR pass isn't enabled." && renderPass);

  return static_cast<const SSRPass &>(*renderPass);
}

void RenderSystem::resizeViewport(unsigned int width, unsigned int height)
{
  m_sceneWidth = width;
  m_sceneHeight = height;

  Renderer::resizeViewport(0, 0, m_sceneWidth, m_sceneHeight);

  if (m_window)
    m_window->resize(m_sceneWidth, m_sceneHeight);

  m_cameraEntity.getComponent<Camera>().resizeViewport(m_sceneWidth,
                                                       m_sceneHeight);

  for (RenderPassPtr &renderPass : m_renderPasses)
  {
    if (renderPass)
      renderPass->resize(width, height);
  }
}

void RenderSystem::createWindow(unsigned int width, unsigned int height,
                                const std::string &title)
{
  m_window = Window::create(width, height, title);
}

void RenderSystem::enableGeometryPass(VertexShader vertShader,
                                      FragmentShader fragShader)
{
  m_renderPasses[static_cast<std::size_t>(RenderPassType::GEOMETRY)] =
      std::make_unique<GeometryPass>(m_sceneWidth, m_sceneHeight,
                                     std::move(vertShader),
                                     std::move(fragShader));
}

void RenderSystem::enableSSRPass(FragmentShader fragShader)
{
  m_renderPasses[static_cast<std::size_t>(RenderPassType::SSR)] =
      std::make_unique<SSRPass>(m_sceneWidth, m_sceneHeight,
                                std::move(fragShader));
}

void RenderSystem::disableGeometryPass()
{
  m_renderPasses[static_cast<std::size_t>(RenderPassType::GEOMETRY)].reset();
}
void RenderSystem::disableSSRPass()
{
  m_renderPasses[static_cast<std::size_t>(RenderPassType::SSR)].reset();
}

void RenderSystem::linkEntity(const EntityPtr &entity)
{
  System::linkEntity(entity);

  if (entity->hasComponent<Mesh>())
    entity->getComponent<Mesh>().load(m_renderPasses.front()->getProgram());

  if (entity->hasComponent<Light>())
    updateLights();
}

bool RenderSystem::update(float deltaTime)
{
  assert("Error: Geometry pass must be enabled for the RenderSystem to be "
         "updated." &&
         m_renderPasses.front());

  m_renderPasses.front()->getProgram().use();

  auto &camera = m_cameraEntity.getComponent<Camera>();
  auto &camTransform = m_cameraEntity.getComponent<Transform>();

  Mat4f viewProjMat;

  if (camTransform.hasUpdated())
  {
    if (camera.getCameraType() == CameraType::LOOK_AT)
    {
      camera.computeLookAt(camTransform.getPosition());
    }
    else
    {
      camera.computeViewMatrix(camTransform.computeTranslationMatrix(true),
                               camTransform.getRotation().inverse());
    }

    camera.computeInverseViewMatrix();

    const Mat4f &viewMat = camera.getViewMatrix();
    viewProjMat = viewMat * camera.getProjectionMatrix();

    sendCameraMatrices(viewProjMat);

    camTransform.setUpdated(false);
  }
  else
  {
    viewProjMat = camera.getViewMatrix() * camera.getProjectionMatrix();
  }

  for (auto &entity : m_entities)
  {
    if (entity->isEnabled())
    {
      if (entity->hasComponent<Mesh>() && entity->hasComponent<Transform>())
      {
        const Mat4f modelMat =
            entity->getComponent<Transform>().computeTransformMatrix();

        const ShaderProgram &geometryProgram =
            m_renderPasses.front()->getProgram();

        geometryProgram.sendUniform("uniModelMatrix", modelMat);
        geometryProgram.sendUniform("uniMvpMatrix", modelMat * viewProjMat);

        entity->getComponent<Mesh>().draw(geometryProgram);
      }
    }
  }

  if (m_cubemap)
    m_cubemap->draw(camera);

#if !defined(NDEBUG)
  Renderer::checkErrors();
#endif

  if (m_window)
    return m_window->run(deltaTime);

  return true;
}

void RenderSystem::sendViewMatrix(const Mat4f &viewMat) const
{
  m_cameraUbo.sendData(viewMat, 0);
}
void RenderSystem::sendInverseViewMatrix(const Mat4f &invViewMat) const
{
  m_cameraUbo.sendData(invViewMat, sizeof(Mat4f));
}
void RenderSystem::sendProjectionMatrix(const Mat4f &projMat) const
{
  m_cameraUbo.sendData(projMat, sizeof(Mat4f) * 2);
}
void RenderSystem::sendInverseProjectionMatrix(const Mat4f &invProjMat) const
{
  m_cameraUbo.sendData(invProjMat, sizeof(Mat4f) * 3);
}
void RenderSystem::sendViewProjectionMatrix(const Mat4f &viewProjMat) const
{
  m_cameraUbo.sendData(viewProjMat, sizeof(Mat4f) * 4);
}
void RenderSystem::sendCameraPosition(const Vec3f &cameraPos) const
{
  m_cameraUbo.sendData(cameraPos, sizeof(Mat4f) * 5);
}

void RenderSystem::sendCameraMatrices(const Mat4f &viewProjMat) const
{
  const auto &camera = m_cameraEntity.getComponent<Camera>();

  m_cameraUbo.bind();
  sendViewMatrix(camera.getViewMatrix());
  sendInverseViewMatrix(camera.getInverseViewMatrix());
  sendProjectionMatrix(camera.getProjectionMatrix());
  sendInverseProjectionMatrix(camera.getInverseProjectionMatrix());
  sendViewProjectionMatrix(viewProjMat);
  sendCameraPosition(m_cameraEntity.getComponent<Transform>().getPosition());
}

void RenderSystem::sendCameraMatrices() const
{
  const auto &camera = m_cameraEntity.getComponent<Camera>();
  sendCameraMatrices(camera.getViewMatrix() * camera.getProjectionMatrix());
}

void RenderSystem::updateLight(const Entity *entity,
                               std::size_t lightIndex) const
{
  assert("Error: Geometry pass must be enabled for the RenderSystem's lights "
         "to be updated." &&
         m_renderPasses.front());

  const ShaderProgram &geometryProgram = getGeometryPass().getProgram();

  geometryProgram.use();

  const std::string strBase = "uniLights[" + std::to_string(lightIndex) + "].";

  const std::string posStr = strBase + "position";
  const std::string colorStr = strBase + "color";
  const std::string energyStr = strBase + "energy";
  const std::string angleStr = strBase + "angle";

  const auto &lightComp = entity->getComponent<Light>();
  Vec4f homogeneousPos(entity->getComponent<Transform>().getPosition(), 1.f);

  if (lightComp.getType() == LightType::DIRECTIONAL)
  {
    homogeneousPos[3] = 0.f;
    geometryProgram.sendUniform(strBase + "direction",
                                lightComp.getDirection());
  }

  geometryProgram.sendUniform(posStr, homogeneousPos);
  geometryProgram.sendUniform(colorStr, lightComp.getColor());
  geometryProgram.sendUniform(energyStr, lightComp.getEnergy());
  geometryProgram.sendUniform(angleStr, lightComp.getAngle());
}

void RenderSystem::updateLights() const
{
  assert("Error: Geometry pass must be enabled for the RenderSystem's lights "
         "to be updated." &&
         m_renderPasses.front());

  if (!m_renderPasses.front())
    return;

  std::size_t lightCount = 0;

  for (const auto &entity : m_entities)
  {
    if (entity->hasComponent<Light>())
    {
      updateLight(entity, lightCount);
      ++lightCount;
    }
  }

  getGeometryPass().getProgram().sendUniform(
      "uniLightCount", static_cast<unsigned int>(lightCount));
}
void RenderSystem::removeCubemap() { m_cubemap.reset(); }

void RenderSystem::updateShaders() const
{
  assert("Error: Geometry pass must be enabled for the RenderSystem's shaders "
         "to be updated." &&
         m_renderPasses.front());

  for (const RenderPassPtr &renderPass : m_renderPasses)
  {
    if (renderPass)
      renderPass->getProgram().updateShaders();
  }

  sendCameraMatrices();
  updateLights();

  for (const auto &entity : m_entities)
  {
    if (entity->hasComponent<Mesh>())
      entity->getComponent<Mesh>().load(getGeometryPass().getProgram());
  }
}

void RenderSystem::saveToImage(const std::string &fileName,
                               TextureFormat format) const
{
  ImageColorspace colorspace = ImageColorspace::RGB;
  TextureDataType dataType = TextureDataType::UBYTE;

  switch (format)
  {
  case TextureFormat::DEPTH:
    colorspace = ImageColorspace::DEPTH;
    dataType = TextureDataType::FLOAT;
    break;

  case TextureFormat::RGBA:
  case TextureFormat::BGRA:
    colorspace = ImageColorspace::RGBA;
    break;

  default:
    break;
  }

  Image img(m_sceneWidth, m_sceneHeight, colorspace);
  Renderer::recoverFrame(m_sceneWidth, m_sceneHeight, format, dataType,
                         img.getDataPtr());

  img.save(fileName, true);
}

void RenderSystem::destroy()
{
  if (m_window)
    m_window->setShouldClose();
}

void RenderSystem::initialize()
{
  Renderer::initialize();

  m_cameraEntity.addComponent<Camera>();
  m_cameraEntity.addComponent<Transform>();

  m_acceptedComponents.setBit(Component::getId<Mesh>());
  m_acceptedComponents.setBit(Component::getId<Light>());

  m_cameraUbo.bindBufferBase(0);

  for (std::size_t passIndex = 1; passIndex < m_renderPasses.size();
       ++passIndex)
  {
    const RenderPassPtr &pass = m_renderPasses[passIndex];

    if (pass)
    {
      const ShaderProgram &passProgram = pass->getProgram();

      pass->getFramebuffer().initBuffers(passProgram);
      m_cameraUbo.bindUniformBlock(passProgram, "uboCameraMatrices", 0);
    }
  }

  sendCameraMatrices();
}

void RenderSystem::initialize(unsigned int sceneWidth, unsigned int sceneHeight)
{
  initialize();
  resizeViewport(sceneWidth, sceneHeight);
}

  //--------------------WindowObserver begin --------------------
void RenderSystem::onFramebufResize(int32_t width, int32_t height) 
{

  m_sceneWidth = width;
  m_sceneHeight = height;

  Renderer::resizeViewport(0, 0, m_sceneWidth, m_sceneHeight);

  // if (m_window)
  //   m_window->resize(m_sceneWidth, m_sceneHeight);

  m_cameraEntity.getComponent<Camera>().resizeViewport(m_sceneWidth,
                                                       m_sceneHeight);

  for (RenderPassPtr &renderPass : m_renderPasses)
  {
    if (renderPass)
      renderPass->resize(width, height);
  }
}
  //--------------------WindowObserver end--------------------

} // namespace Raz
