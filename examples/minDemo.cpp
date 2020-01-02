#include "RaZ/RaZ.hpp"

using namespace std::literals;

void just_for_test()
{
  Raz::Vec3f a({-1.0f, -1.0f, 0.0f});
  Raz::Vec3f b({1.0f, -1.0f, 0.0f});
  Raz::Vec3f c({0.0f, 1.0f, 0.0f});

  Raz::Vec3f p({0.0f, 0.0f, 0.0f});
  Raz::Vec3f p1({-2.0f, 0.0f, 0.0f});

  bool is_inside = Raz::isInsideTriangle(a,b,c,p);
  auto is_inside1 = Raz::isInsideTriangle(a,b,c,p1);
  if (is_inside)
  {
    printf("");
  }

}

bool r_button_down = false;

int main() {
  // just_for_test();
  Raz::Application app;
  Raz::World& world = app.addWorld(Raz::World(2));

  auto& render = world.addSystem<Raz::RenderSystem>(1280, 720, "RaZ");
  render.enableGeometryPass(Raz::VertexShader(RAZ_ROOT + "shaders/vert.glsl"s), Raz::FragmentShader(RAZ_ROOT + "shaders/cook-torrance.glsl"s));

  render.getCameraEntity().getComponent<Raz::Transform>().setPosition(Raz::Vec3f({ 0.f, 0.f, -5.f }));

  Raz::Entity& mesh = world.addEntityWithComponent<Raz::Transform>();
  mesh.addComponent<Raz::Mesh>(RAZ_ROOT + "assets/meshes/ball.obj"s);

  // Raz::Entity& mesh = world.addEntityWithComponent<Raz::Transform>();
  // mesh.addComponent<Raz::Plane>(Raz::Vec3f{0.0f, 0.0f, 0.0f});

  // Raz::Entity& mesh = world.addEntityWithComponent<Raz::Transform>();
  // mesh.addComponent<Raz::Mesh>(RAZ_ROOT + "assets/meshes/dungeon.obj"s);

  Raz::Entity& light = world.addEntityWithComponent<Raz::Transform>();
  light.addComponent<Raz::Light>(Raz::LightType::DIRECTIONAL,   // Type
                                 Raz::Vec3f({ 0.f, 0.f, 1.f }), // Direction
                                 1.f,                           // Energy
                                 Raz::Vec3f(1.f));              // Color (R/G/B)

  render.getWindow().addKeyCallback(Raz::Keyboard::ESCAPE, [&app] (float /* deltaTime */) { app.quit(); });
  //i need to control
  //first get the camora component
  auto& camera_ent = render.getCameraEntity();
  auto& camera_trans = camera_ent.getComponent<Raz::Transform>();
  float camera_speed = 1.0f;
  //set the camera speed
  render.getWindow().addKeyCallback(Raz::Keyboard::LEFT_SHIFT
                      , [&camera_speed](float delta_time){
                        camera_speed = 2.0f;
                      }
                      , Raz::Input::ONCE
                      , [&camera_speed](){camera_speed = 1.0f;});
  //move towards y
  render.getWindow().addKeyCallback(Raz::Keyboard::SPACE
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move(0.f, (10.0f*delta_time)*camera_speed, 0.0f);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::V
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move(0.f, (-10.0f*delta_time)*camera_speed, 0.0f);
                      });
  //w
  render.getWindow().addKeyCallback(Raz::Keyboard::W
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move(0.f, 0.0f, (10.0f*delta_time)*camera_speed);
                      });
  //s                    
  render.getWindow().addKeyCallback(Raz::Keyboard::S
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move(0.f, 0.0f, (-10.0f*delta_time)*camera_speed);
                      });
  //a
  render.getWindow().addKeyCallback(Raz::Keyboard::A
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move((-10.0f*delta_time)*camera_speed, 0.0f, 0.0f);
                      });
  //d                      
  render.getWindow().addKeyCallback(Raz::Keyboard::D
                      , [&camera_trans, &camera_speed](float delta_time) {
                        camera_trans.move((10.0f*delta_time)*camera_speed, 0.0f, 0.0f);
                      });
  //rotate the mesh, not the camera
  auto& mesh_trans = mesh.getComponent<Raz::Transform>();
  render.getWindow().addKeyCallback(Raz::Keyboard::Q
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        // camera_trans.move((10.0f*delta_time)*camera_speed, 0.0f, 0.0f);
                        mesh_trans.rotate(90.0_deg*delta_time, Raz::Axis::Z);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::E
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        mesh_trans.rotate(-90.0_deg*delta_time, Raz::Axis::Z);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::Z
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        // camera_trans.move((10.0f*delta_time)*camera_speed, 0.0f, 0.0f);
                        mesh_trans.rotate(90.0_deg*delta_time, Raz::Axis::Y);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::C
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        mesh_trans.rotate(-90.0_deg*delta_time, Raz::Axis::Y);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::R
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        // camera_trans.move((10.0f*delta_time)*camera_speed, 0.0f, 0.0f);
                        mesh_trans.rotate(90.0_deg*delta_time, Raz::Axis::X);
                      });
  render.getWindow().addKeyCallback(Raz::Keyboard::T
                      , [&mesh_trans, &camera_speed](float delta_time) {
                        mesh_trans.rotate(-90.0_deg*delta_time, Raz::Axis::X);
                      });
  
  render.getWindow().addMouseButtonCallback(Raz::Mouse::RIGHT_CLICK
                        , [  ](double val){ r_button_down = true;}
                        , Raz::Input::ONCE
                        , []() { r_button_down = false;});
  render.getWindow().addMouseMoveCallback([&mesh_trans,&camera_speed ](double x, double y) {
    if (r_button_down)
    {
    mesh_trans.rotate(90.0_deg*y*camera_speed*0.01
                      ,90.0_deg*x*camera_speed*0.01
                      , 0.0_deg );

    }
  });

  while (app.run());

  return EXIT_SUCCESS;
}
