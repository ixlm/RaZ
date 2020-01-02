#include "RaZ/RaZ.hpp"

using namespace std::literals;

int main() {
  Raz::Application app;
  Raz::World& world = app.addWorld(Raz::World(10));

  auto& renderSystem = world.addSystem<Raz::RenderSystem>(1280, 720, "RaZ", 2);
  renderSystem.enableGeometryPass(Raz::VertexShader(RAZ_ROOT + "shaders/vert.glsl"s),
                                  Raz::FragmentShader(RAZ_ROOT + "shaders/cook-torrance.glsl"s));

  Raz::Window& window = renderSystem.getWindow();

  auto overlay = window.addOverlay("default");
  // window.enableOverlay();
  window.setIcon(RAZ_ROOT + "assets/icons/RaZ_logo_128.png"s);

  Raz::Entity& camera = renderSystem.getCameraEntity();
  auto& cameraComp    = camera.getComponent<Raz::Camera>();
  auto& cameraTrans   = camera.getComponent<Raz::Transform>();
  cameraTrans.setPosition(Raz::Vec3f({ 0.f, 0.f, -5.f }));

  Raz::Entity& mesh = world.addEntity();
  auto& meshTrans   = mesh.addComponent<Raz::Transform>();
  mesh.addComponent<Raz::Mesh>(RAZ_ROOT + "assets/meshes/shield.obj"s);

  meshTrans.scale(0.2f);
  meshTrans.rotate(180.0_deg, Raz::Axis::Y);

  renderSystem.setCubemap(Raz::Cubemap::create(RAZ_ROOT + "assets/skyboxes/clouds_right.png"s, RAZ_ROOT + "assets/skyboxes/clouds_left.png"s,
                                               RAZ_ROOT + "assets/skyboxes/clouds_top.png"s,   RAZ_ROOT + "assets/skyboxes/clouds_bottom.png"s,
                                               RAZ_ROOT + "assets/skyboxes/clouds_front.png"s, RAZ_ROOT + "assets/skyboxes/clouds_back.png"s));

  Raz::Entity& light = world.addEntity();
  /*auto& lightComp = light.addComponent<Raz::Light>(Raz::LightType::POINT, // Type
                                                   1.f,                   // Energy
                                                   Raz::Vec3f(1.f));      // Color(RGB)*/
  auto& lightComp = light.addComponent<Raz::Light>(Raz::LightType::DIRECTIONAL,   // Type
                                                   Raz::Vec3f({ 0.f, 0.f, 1.f }), // Direction
                                                   1.f,                           // Energy
                                                   Raz::Vec3f(1.f));              // Color (RGB)
  auto& lightTrans = light.addComponent<Raz::Transform>(Raz::Vec3f({ 0.f, 1.f, 0.f }));

  window.addKeyCallback(Raz::Keyboard::R, [&mesh] (float /* deltaTime */) { mesh.disable(); },
                        Raz::Input::ONCE,
                        [&mesh] () { mesh.enable(); });

  window.addKeyCallback(Raz::Keyboard::ESCAPE, [&app] (float /* deltaTime */) { app.quit(); });

  // Camera controls
  float cameraSpeed = 1.f;
  window.addKeyCallback(Raz::Keyboard::LEFT_SHIFT,
                        [&cameraSpeed] (float /* deltaTime */) { cameraSpeed = 2.f; },
                        Raz::Input::ONCE,
                        [&cameraSpeed] () { cameraSpeed = 1.f; });
  window.addKeyCallback(Raz::Keyboard::SPACE, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move(0.f, (10.f * deltaTime) * cameraSpeed, 0.f);
  });
  window.addKeyCallback(Raz::Keyboard::V, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move(0.f, (-10.f * deltaTime) * cameraSpeed, 0.f);
  });
  window.addKeyCallback(Raz::Keyboard::W, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move(0.f, 0.f, (10.f * deltaTime) * cameraSpeed);
  });
  window.addKeyCallback(Raz::Keyboard::S, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move(0.f,  0.f, (-10.f * deltaTime) * cameraSpeed);
  });
  window.addKeyCallback(Raz::Keyboard::A, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move((-10.f * deltaTime) * cameraSpeed, 0.f, 0.f);
  });
  window.addKeyCallback(Raz::Keyboard::D, [&cameraTrans, &cameraSpeed] (float deltaTime) {
    cameraTrans.move((10.f * deltaTime) * cameraSpeed, 0.f, 0.f);
  });

  window.addKeyCallback(Raz::Keyboard::NUM8, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(-90.0_deg * deltaTime, Raz::Axis::X); // Looking up
  });
  window.addKeyCallback(Raz::Keyboard::NUM2, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(90.0_deg * deltaTime, Raz::Axis::X); // Looking down
  });
  window.addKeyCallback(Raz::Keyboard::NUM4, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(-90.0_deg * deltaTime, Raz::Axis::Y); // Looking left
  });
  window.addKeyCallback(Raz::Keyboard::NUM6, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(90.0_deg * deltaTime, Raz::Axis::Y); // Looking right
  });
  // DO A BARREL ROLL
  window.addKeyCallback(Raz::Keyboard::Q, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(90.0_deg * deltaTime, Raz::Axis::Z); // Roll to the left
  });
  window.addKeyCallback(Raz::Keyboard::E, [&cameraTrans] (float deltaTime) {
    cameraTrans.rotate(-90.0_deg * deltaTime, Raz::Axis::Z); // Roll to the right
  });

  // Mesh controls
  window.addKeyCallback(Raz::Keyboard::T, [&meshTrans] (float deltaTime) { meshTrans.move(0.f, 0.f,  10.f * deltaTime); });
  window.addKeyCallback(Raz::Keyboard::G, [&meshTrans] (float deltaTime) { meshTrans.move(0.f, 0.f, -10.f * deltaTime); });
  window.addKeyCallback(Raz::Keyboard::F, [&meshTrans] (float deltaTime) { meshTrans.move(-10.f * deltaTime, 0.f, 0.f); });
  window.addKeyCallback(Raz::Keyboard::H, [&meshTrans] (float deltaTime) { meshTrans.move( 10.f * deltaTime, 0.f, 0.f); });

  window.addKeyCallback(Raz::Keyboard::X, [&meshTrans] (float /* deltaTime */) { meshTrans.scale(0.5f); }, Raz::Input::ONCE);
  window.addKeyCallback(Raz::Keyboard::C, [&meshTrans] (float /* deltaTime */) { meshTrans.scale(2.f); }, Raz::Input::ONCE);

  window.addKeyCallback(Raz::Keyboard::UP,    [&meshTrans] (float deltaTime) { meshTrans.rotate(-90.0_deg * deltaTime, Raz::Axis::X); });
  window.addKeyCallback(Raz::Keyboard::DOWN,  [&meshTrans] (float deltaTime) { meshTrans.rotate(90.0_deg * deltaTime, Raz::Axis::X); });
  window.addKeyCallback(Raz::Keyboard::LEFT,  [&meshTrans] (float deltaTime) { meshTrans.rotate(-90.0_deg * deltaTime, Raz::Axis::Y); });
  window.addKeyCallback(Raz::Keyboard::RIGHT, [&meshTrans] (float deltaTime) { meshTrans.rotate(90.0_deg * deltaTime, Raz::Axis::Y); });

  // Light controls
  window.addKeyCallback(Raz::Keyboard::I, [&lightTrans, &renderSystem] (float deltaTime) {
    lightTrans.translate(0.f, 0.f, 10.f * deltaTime);
    renderSystem.updateLights();
  });
  window.addKeyCallback(Raz::Keyboard::K, [&lightTrans, &renderSystem] (float deltaTime) {
    lightTrans.translate(0.f, 0.f, -10.f * deltaTime);
    renderSystem.updateLights();
  });
  window.addKeyCallback(Raz::Keyboard::J, [&lightTrans, &renderSystem] (float deltaTime) {
    lightTrans.translate(-10.f * deltaTime, 0.f, 0.f);
    renderSystem.updateLights();
  });
  window.addKeyCallback(Raz::Keyboard::L, [&lightTrans, &renderSystem] (float deltaTime) {
    lightTrans.translate(10.f * deltaTime, 0.f, 0.f);
    renderSystem.updateLights();
  });
  window.addKeyCallback(Raz::Keyboard::PAGEUP, [&lightComp, &renderSystem] (float deltaTime) {
    lightComp.setEnergy(lightComp.getEnergy() + 1.f * deltaTime);
    renderSystem.updateLights();
  });
  window.addKeyCallback(Raz::Keyboard::PAGEDOWN, [&lightComp, &renderSystem] (float deltaTime) {
    lightComp.setEnergy(std::max(0.f, lightComp.getEnergy() - 1.f * deltaTime));
    renderSystem.updateLights();
  });

  window.addMouseButtonCallback(Raz::Mouse::RIGHT_CLICK, [&world, &cameraTrans] (float /* deltaTime */) {
    auto& newLight = world.addEntityWithComponent<Raz::Light>(Raz::LightType::POINT, // Type (point light)
                                                              10.f);                 // Energy
    newLight.addComponent<Raz::Transform>(cameraTrans.getPosition());
  }, Raz::Input::ONCE);

  window.addKeyCallback(Raz::Keyboard::F5, [&renderSystem] (float /* deltaTime */) { renderSystem.updateShaders(); });

  // Mouse callbacks
  window.addMouseScrollCallback([&cameraComp] (double /* xOffset */, double yOffset) {
    cameraComp.setFieldOfView(Raz::Degreesf(std::max(15.f,
                                            std::min(90.f, (Raz::Degreesf(cameraComp.getFieldOfView()) + static_cast<float>(-yOffset) * 2.f).value))));
  });
  window.addMouseMoveCallback([&cameraTrans, &window] (double xMove, double yMove) {
    // Dividing move by window size to scale between -1 and 1
    cameraTrans.rotate(90.0_deg * static_cast<float>(yMove) / window.getHeight(),
                       90.0_deg * static_cast<float>(xMove) / window.getWidth(),
                       0.0_deg);
  });

  window.disableCursor(); // Disabling mouse cursor to allow continuous rotationcameraTranss
  window.addKeyCallback(Raz::Keyboard::LEFT_ALT,
                        [&window] (float /* deltaTime */) { window.showCursor(); },
                        Raz::Input::ONCE,
                        [&window] () { window.disableCursor(); });

  // Overlay features
  overlay->addLabel("RaZ - Full demo");
  overlay->addSeparator();
  overlay->addCheckbox("Enable face culling",
                      [&window] () { window.enableFaceCulling(); },
                      [&window] () { window.disableFaceCulling(); },
                      true);
  overlay->addCheckbox("Enable vertical sync",
                      [&window] () { window.enableVerticalSync(); },
                      [&window] () { window.disableVerticalSync(); },
                      window.recoverVerticalSyncState());
  overlay->addSeparator();
  overlay->addFrameTime("Frame time: %.3f ms/frame"); // Frame time's & FPS counter's texts must be formatted
  overlay->addFpsCounter("FPS: %.1f");

  while (app.run());

  return EXIT_SUCCESS;
}
