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
std::unordered_map<uint32_t, OverlayRenderer > g_element_render_map;

void registerOverlayRender(OverlayElementType type, OverlayRenderer renderer)
{
  g_element_render_map[(uint32_t)type] = [renderer](OverlayElement* elem) { renderer(elem);};
}

void unregisterOverlayRender(OverlayElementType type)
{
  auto it = g_element_render_map.find((uint32_t)type);
  if (it != g_element_render_map.end())
    g_element_render_map.erase(it);
}

void OverlayLabel::registerRender()
{
  registerOverlayRender(OverlayElementType::LABEL, [](OverlayElement *elem) {
    ImGui::TextUnformatted(elem->getName().c_str());
  });
}

void OverlayButton::registerRender()
{
  registerOverlayRender(OverlayElementType::BUTTON, [](OverlayElement *elem) {
    OverlayButton *btn = dynamic_cast<OverlayButton *>(elem);
    if (ImGui::Button(btn->getName().c_str()))
    {
      btn->m_actionClick();
    }
  });
}

void OverlayCheckbox::registerRender()
{
  registerOverlayRender(OverlayElementType::CHECKBOX, [](OverlayElement *elem) {
    auto* checkbox = dynamic_cast<OverlayCheckbox*>(elem);
    const bool pre_val = checkbox->m_isChecked;
    ImGui::Checkbox(checkbox->m_label.c_str(), &checkbox->m_isChecked);
    if (checkbox->m_isChecked != pre_val)
      checkbox->m_isChecked ? checkbox->m_actionOn() : checkbox->m_actionOff();
  });
}

void OverlayTextbox::registerRender()
{
  static auto callback = [](ImGuiTextEditCallbackData* data)
  {
    auto* textbox = reinterpret_cast<OverlayTextbox*>(data->UserData);
    textbox->m_text += static_cast<char>(data->EventChar);
    if (textbox->m_callback)
      textbox->m_callback(textbox->m_text);
    return 0;
  };
  registerOverlayRender(OverlayElementType::TEXTBOX, [](OverlayElement *elem) {
    auto* textbox = dynamic_cast<OverlayTextbox*>(elem);
    ImGui::InputText(textbox->getName().c_str(), textbox->m_text.data()
                      , textbox->m_text.capacity()
                      , ImGuiInputTextFlags_CallbackCharFilter, 
                      nullptr
                      ,textbox);
  });

}

void OverlaySeparator::registerRender()
{
  registerOverlayRender(OverlayElementType::SEPARATOR, [](OverlayElement *elem) {
    ImGui::Separator();
  });
}

void OverlayFrameTime::registerRender()
{
  registerOverlayRender(OverlayElementType::FRAME_TIME, [](OverlayElement *elem) {
    // auto* frame = dynamic_cast<OverlayFrameTime*>(elem);
    ImGui::Text(elem->getName().c_str(), 1000.0f/ImGui::GetIO().Framerate );
  });
}

void OverlayFpsCounter::registerRender()
{
  registerOverlayRender(OverlayElementType::FPS_COUNTER, [](OverlayElement *elem) {
    // auto* frame = dynamic_cast<OverlayFrameTime*>(elem);
    ImGui::Text(elem->getName().c_str(), ImGui::GetIO().Framerate );
  });

}

void OverlaySameLine::registerRender()
{
  registerOverlayRender(OverlayElementType::SAMELINE, [](OverlayElement *elem) {
    ImGui::SameLine();
  });
}

void OverlayChildFrame::registerRender()
{
  registerOverlayRender(OverlayElementType::CHILD_FRAME, [](OverlayElement *elem) {
    auto* frame = dynamic_cast<OverlayChildFrame*>(elem);
    frame->render();
  });
}
void OverlayFrame::registerRender()
{
  registerOverlayRender(OverlayElementType::FRAME, [](OverlayElement *elem) {
    auto* frame = dynamic_cast<OverlayFrame*>(elem);
    frame->render();
  });
}

// registerOverlayRender(OverlayElementType::LABEL, )


void OverlayFrame::render()
{
  ImGui::Begin(m_label.c_str(), &m_show_close, m_win_flags);
  ImGui::SetWindowPos(m_pos, m_pos_cond);
  ImGui::SetWindowSize(m_size, m_size_cond);
  for (auto &item : m_elements)
  {
    auto *element = item.second;

    auto it = g_element_render_map.find((uint32_t)element->getType());
    if (it != g_element_render_map.end())
    {
      it->second(element);
    }
    

  }
  ImGui::End();
}

void OverlayChildFrame::render()
{
  ImGui::BeginChild(m_label.c_str(), m_size, m_has_boder, m_win_flags);
  for (auto &item : m_elements)
  {
    auto *element = item.second;

    auto it = g_element_render_map.find((uint32_t)element->getType());
    if (it != g_element_render_map.end())
    {
      it->second(element);
    }

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
    
    auto it = g_element_render_map.find((uint32_t)element->getType());
    if (it != g_element_render_map.end())
    {
      it->second(element);
    }
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
