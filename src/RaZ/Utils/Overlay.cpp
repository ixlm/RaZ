#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "RaZ/Utils/Overlay.hpp"

namespace Raz {

// Overlay::Overlay(GLFWwindow* window) {
//   IMGUI_CHECKVERSION();

//   ImGui::CreateContext();
//   ImGui::StyleColorsDark();

//   ImGui_ImplGlfw_InitForOpenGL(window, true);
//   ImGui_ImplOpenGL3_Init("#version 330 core");
// }

void OverlayFrame::render()
{
  ImGui::Begin(m_label.c_str(), &m_show_close, m_win_flags);
  ImGui::SetWindowPos(m_pos, m_pos_cond);
  ImGui::SetWindowSize(m_size, m_size_cond);
  for (auto& item: m_elements) {
    auto* element = item.second;
    
    switch (element->getType()) {
      case OverlayElementType::LABEL:
        ImGui::TextUnformatted(element->getName().c_str());
        break;

      case OverlayElementType::BUTTON:
      {
        const auto& button = static_cast<OverlayButton&>(*element);

        if (ImGui::Button(button.getName().c_str()))
          button.m_actionClick();

        break;
      }

      case OverlayElementType::CHECKBOX:
      {
        auto& checkbox = static_cast<OverlayCheckbox&>(*element);
        const bool prevValue = checkbox.m_isChecked;

        ImGui::Checkbox(checkbox.m_label.c_str(), &checkbox.m_isChecked);

        if (checkbox.m_isChecked != prevValue) {
          if (checkbox.m_isChecked)
            checkbox.m_actionOn();
          else
            checkbox.m_actionOff();
        }

        break;
      }

      case OverlayElementType::TEXTBOX:
      {
        static auto callback = [] (ImGuiTextEditCallbackData* data) {
          auto& textbox = *static_cast<OverlayTextbox*>(data->UserData);

          textbox.m_text += static_cast<char>(data->EventChar);

          if (textbox.m_callback)
            textbox.m_callback(textbox.m_text);

          return 0;
        };

        auto& textbox = static_cast<OverlayTextbox&>(*element);

        ImGui::InputText(textbox.m_label.c_str(),
                         textbox.m_text.data(),
                         textbox.m_text.capacity(),
                         ImGuiInputTextFlags_CallbackCharFilter,
                         callback,
                         &textbox);

        break;
      }

      case OverlayElementType::SEPARATOR:
        ImGui::Separator();
        break;

      case OverlayElementType::FRAME_TIME:
        ImGui::Text(element->getName().c_str(), static_cast<double>(1000.f / ImGui::GetIO().Framerate));
        break;

      case OverlayElementType::FPS_COUNTER:
        ImGui::Text(element->getName().c_str(), static_cast<double>(ImGui::GetIO().Framerate));
        break;
      case OverlayElementType::SAMELINE:
        ImGui::SameLine();
        break;

      default:
        break;
    }
  }
  ImGui::End();

}

Overlay::Overlay( GLFWwindow* window ) : m_status(Overlay::OverlayStatus::OS_ENABLED)
{
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

// void Overlay::setName(const std::string& name)
// {
//   m_name = name;
// }
// std::string& Overlay::getName()
// {
//   return m_name;
// }

/**
@brief:xlm, get the overlay status,
@param:
@return: 1 enabled, 2 disabled
*/
Overlay::OverlayStatus Overlay::getStatus() { return m_status; }
void Overlay::setStatus(Overlay::OverlayStatus status) { m_status = status; }

// void Overlay::addLabel(std::string label) {
//   m_elements.emplace_back(OverlayLabel::create(std::move(label)));
// }

// void Overlay::addButton(std::string label, std::function<void()> actionClick) {
//   m_elements.emplace_back(OverlayButton::create(std::move(label), std::move(actionClick)));
// }

// void Overlay::addCheckbox(std::string label, std::function<void()> actionOn, std::function<void()> actionOff, bool initVal) {
//   m_elements.emplace_back(OverlayCheckbox::create(std::move(label), std::move(actionOn), std::move(actionOff), initVal));
// }

// void Overlay::addTextbox(std::string label, std::function<void(const std::string&)> callback) {
//   m_elements.emplace_back(OverlayTextbox::create(std::move(label), std::move(callback)));
//   static_cast<OverlayTextbox&>(*m_elements.back()).m_text.reserve(64);
// }

// void Overlay::addSeparator() {
//   m_elements.emplace_back(OverlaySeparator::create());
// }

// void Overlay::addFrameTime(std::string formattedLabel) {
//   m_elements.emplace_back(OverlayFrameTime::create(std::move(formattedLabel)));
// }

// void Overlay::addFpsCounter(std::string formattedLabel) {
//   m_elements.emplace_back(OverlayFpsCounter::create(std::move(formattedLabel)));
// }

bool Overlay::hasKeyboardFocus() const {
  return ImGui::GetIO().WantCaptureKeyboard;
}

void Overlay::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // for (auto& element : m_elements) {
  for (auto& item: m_elements) {
    auto* element = item.second;
    
    switch (element->getType()) {
      case OverlayElementType::LABEL:
        ImGui::TextUnformatted(element->m_label.c_str());
        break;

      case OverlayElementType::BUTTON:
      {
        const auto& button = static_cast<OverlayButton&>(*element);

        if (ImGui::Button(button.m_label.c_str()))
          button.m_actionClick();

        break;
      }

      case OverlayElementType::CHECKBOX:
      {
        auto& checkbox = static_cast<OverlayCheckbox&>(*element);
        const bool prevValue = checkbox.m_isChecked;

        ImGui::Checkbox(checkbox.m_label.c_str(), &checkbox.m_isChecked);

        if (checkbox.m_isChecked != prevValue) {
          if (checkbox.m_isChecked)
            checkbox.m_actionOn();
          else
            checkbox.m_actionOff();
        }

        break;
      }

      case OverlayElementType::TEXTBOX:
      {
        static auto callback = [] (ImGuiTextEditCallbackData* data) {
          auto& textbox = *static_cast<OverlayTextbox*>(data->UserData);

          textbox.m_text += static_cast<char>(data->EventChar);

          if (textbox.m_callback)
            textbox.m_callback(textbox.m_text);

          return 0;
        };

        auto& textbox = static_cast<OverlayTextbox&>(*element);

        ImGui::InputText(textbox.m_label.c_str(),
                         textbox.m_text.data(),
                         textbox.m_text.capacity(),
                         ImGuiInputTextFlags_CallbackCharFilter,
                         callback,
                         &textbox);

        break;
      }

      case OverlayElementType::SEPARATOR:
        ImGui::Separator();
        break;

      case OverlayElementType::FRAME_TIME:
        ImGui::Text(element->m_label.c_str(), static_cast<double>(1000.f / ImGui::GetIO().Framerate));
        break;

      case OverlayElementType::FPS_COUNTER:
        ImGui::Text(element->m_label.c_str(), static_cast<double>(ImGui::GetIO().Framerate));
        break;
      case OverlayElementType::SAMELINE:
        ImGui::SameLine();
        break;
      case OverlayElementType::FRAME:
        ((OverlayFrame*)element)->render();
      break;
      default:
        break;
    }
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Overlay::~Overlay() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

  /**
  @brief:xlm, as the seperator(sameline) element has no name, so can not get it by this function
  @param: 
  @return:
  */
OverlayElement* Overlay::getOverlayElementByName(const std::string& name)
{
  auto it = m_elements.find(name);
  if (it != m_elements.end())
    return it->second;
  return nullptr;
}

} // namespace Raz
