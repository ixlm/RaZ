#include "RaZ/Utils/Overlay.hpp"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <functional>
#include <unordered_map>



namespace Raz
{

uint64_t g_no_name_element_seq = time(NULL);

void OverlayLabel::render()
{
 ImGui::TextUnformatted(m_label.c_str());
}

void OverlayButton::render()
{
    if (ImGui::Button(m_label.c_str()))
    {
      m_actionClick();
    }
}


void OverlayCheckbox::render()
{
  auto pre_val = m_isChecked;
  ImGui::Checkbox(m_label.c_str(), &m_isChecked);
  if (m_isChecked != pre_val)
    m_isChecked ? m_actionOn() : m_actionOff();
}


void OverlayTextbox::render()
{
  static auto callback = [](ImGuiTextEditCallbackData* data)
  {
    auto* textbox = reinterpret_cast<OverlayTextbox*>(data->UserData);
    textbox->m_text += static_cast<char>(data->EventChar);
    if (textbox->m_callback)
      textbox->m_callback(textbox->m_text);
    return 0;
  };
  ImGui::InputText(m_label.c_str(), m_text.data()
                      , m_text.capacity()
                      , ImGuiInputTextFlags_CallbackCharFilter, 
                      nullptr
                      ,this);

}

void OverlaySeparator::render()
{
  ImGui::Separator();
}

void OverlayFrameTime::render()
{
 ImGui::Text(m_label.c_str(), 1000.0f/ImGui::GetIO().Framerate );
}

void OverlayFpsCounter::render()
{
  ImGui::Text(m_label.c_str(), ImGui::GetIO().Framerate );

}

void OverlaySameLine::render()
{
    ImGui::SameLine();
}

void OverlayFrame::render()
{
  ImGui::Begin(m_label.c_str(), &m_show_close, m_win_flags);
  ImGui::SetWindowPos(m_pos, m_pos_cond);
  ImGui::SetWindowSize(m_size, m_size_cond);
  for (auto &item : m_elements)
  {
    auto *element = item.second;
    element->render();
  }
  ImGui::End();
}

void OverlayChildFrame::render()
{
  ImGui::BeginChild(m_label.c_str(), m_size, m_has_boder, m_win_flags);
  for (auto &item : m_elements)
  {
    auto *element = item.second;
    element->render();
  }
  ImGui::EndChild();
}

Overlay::Overlay(GLFWwindow *window)
    : m_status(Overlay::OverlayStatus::OS_ENABLED)
{
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

/**
@brief:xlm, get the overlay status,
@param:
@return: 1 enabled, 2 disabled
*/
Overlay::OverlayStatus Overlay::getStatus() { return m_status; }
void Overlay::setStatus(Overlay::OverlayStatus status) { m_status = status; }

bool Overlay::hasKeyboardFocus() const
{
  return ImGui::GetIO().WantCaptureKeyboard;
}

void Overlay::render()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // for (auto& element : m_elements) {
  for (auto &item : m_elements)
  {
    auto *element = item.second;
    element->render();
    
    // auto it = g_element_render_map.find((uint32_t)element->getType());
    // if (it != g_element_render_map.end())
    // {
    //   it->second(element);
    // }
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Overlay::~Overlay()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

/**
@brief:xlm, as the seperator(sameline) element has no name, so can not get it by
this function
@param:
@return:
*/
OverlayElement *Overlay::getOverlayElementByName(const std::string &name)
{
  auto it = m_elements.find(name);
  if (it != m_elements.end())
    return it->second;
  return nullptr;
}

} // namespace Raz
