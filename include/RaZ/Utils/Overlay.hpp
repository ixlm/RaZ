#pragma once

#ifndef RAZ_OVERLAY_HPP
#define RAZ_OVERLAY_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "imgui/imgui.h"
// #include <unordered_map>
#include <map>

// Forward declaration of GLFWwindow, to allow its usage into functions
struct GLFWwindow;

namespace Raz {

#define concat_element_name(T, n) T##n

class Overlay;
using OverlayPtr = std::unique_ptr<Overlay>;

enum class OverlayElementType {
  LABEL,
  BUTTON,
  CHECKBOX,
  TEXTBOX,
  SEPARATOR,
  FRAME_TIME,
  FPS_COUNTER,
  FRAME,
  SAMELINE,
  CHILD_FRAME,
};


  class OverlayElement;
  class OverlayLabel;
  class OverlayButton;
  class OverlayCheckbox;
  class OverlayTextbox;
  class OverlaySeparator;
  class OverlayFrameTime;
  class OverlayFpsCounter;
  class OverlayFrame;
  class OverlaySameLine;
  class OverlayChildFrame;


  class OverlayElement {
    friend Overlay;

  public:
    OverlayElement() = default;
    explicit OverlayElement(std::string label) : m_label{ std::move(label) } {}

    virtual OverlayElementType getType() const = 0;

    virtual ~OverlayElement() = default;
    virtual const std::string& getName() const
    {
      return m_label;
    }

  protected:
    std::string m_label {};
  };



  class OverlayLabel : public OverlayElement {
    friend Overlay; 
    friend OverlayFrame;

  public:
    explicit OverlayLabel(std::string label) : OverlayElement(std::move(label)) {}

    OverlayElementType getType() const override { return OverlayElementType::LABEL; }
  };

  class OverlayButton : public OverlayElement {
    friend Overlay;
    friend OverlayFrame;

  public:
    OverlayButton(std::string label, std::function<void()> actionClick) : OverlayElement(std::move(label)), m_actionClick{ std::move(actionClick) } {}

    OverlayElementType getType() const override { return OverlayElementType::BUTTON; }
  private:
    std::function<void()> m_actionClick {};
  };

  class OverlayCheckbox : public OverlayElement {
    friend Overlay;
    friend OverlayFrame;

  public:
    OverlayCheckbox(std::string label, std::function<void()> actionOn, std::function<void()> actionOff, bool initVal)
      : OverlayElement(std::move(label)), m_actionOn{ std::move(actionOn) }, m_actionOff{ std::move(actionOff) }, m_isChecked{ initVal } {}

    OverlayElementType getType() const override { return OverlayElementType::CHECKBOX; }

  private:
    std::function<void()> m_actionOn {};
    std::function<void()> m_actionOff {};
    bool m_isChecked {};
  };

  class OverlayTextbox : public OverlayElement {
    friend Overlay;
    friend OverlayFrame;

  public:
    OverlayTextbox(std::string label, std::function<void(const std::string&)> callback) : OverlayElement(std::move(label)), m_callback{ std::move(callback) } {}

    OverlayElementType getType() const override { return OverlayElementType::TEXTBOX; }
  private:
    std::string m_text {};
    std::function<void(const std::string&)> m_callback {};
  };

  class OverlaySeparator : public OverlayElement {
  public:
    OverlayElementType getType() const override { return OverlayElementType::SEPARATOR; }
  };

  class OverlayFrameTime : public OverlayElement {
  public:
    explicit OverlayFrameTime(std::string formattedLabel) : OverlayElement(std::move(formattedLabel)) {}

    OverlayElementType getType() const override { return OverlayElementType::FRAME_TIME; }

  };

  class OverlayFpsCounter : public OverlayElement {
  public:
    explicit OverlayFpsCounter(std::string formattedLabel) : OverlayElement(std::move(formattedLabel)) {}

    OverlayElementType getType() const override { return OverlayElementType::FPS_COUNTER; }

  };


  //added by xlm
  class OverlayFrame : public OverlayElement
  {
  public:
    friend Overlay;
    explicit OverlayFrame(const std::string& name, bool show_close, ImGuiWindowFlags flags)
      : OverlayElement(std::move(name))
      , m_show_close(show_close)
      ,m_win_flags(flags) {}
    OverlayElementType getType() const override { return OverlayElementType::FRAME; }

    template <typename T, typename... Args>
    T *addOverlayElement(const std::string &label, Args &&... args)
    {
      auto it = m_elements.find(label);
      if (it != m_elements.end())
      {
        delete it->second;
      }
      auto *elem = new T(label, std::forward<Args>(args)...);
      if (elem)
        m_elements[label] = elem;
      return elem;
    }
    template <typename T, typename... Args>
    T *addNoNameOverlayElement(Args &&... args)
    {
      std::string label(typeid(T).name());
      label.append(std::to_string(m_no_name_element_seq));
      m_no_name_element_seq = (m_no_name_element_seq + 1) % 0xffffffff;
      auto it = m_elements.find(label);
      if (it != m_elements.end())
      {
        delete it->second;
      }
      auto *elem = new T(std::forward<Args>(args)...);
      if (elem)
        m_elements[label] = elem;
      return elem;
    }

    template <typename T> T *addOverlayElement(T *elem)
    {
      auto it = m_elements.find(elem->getLabel());
      if (it != m_elements.end())
      {
        delete it->second;
      }
      m_elements[elem->getLabel()] = elem;
      return elem;
    }

    void setFramePos(const ImVec2 &pos, ImGuiCond cond)
    {
      m_pos = pos;
      m_pos_cond = cond;
    }
    void setFrameSize(const ImVec2& size, ImGuiCond cond)
    {
      m_size = size;
      m_size_cond = cond;
    }
    void render();

  private:
    std::map<std::string, OverlayElement* > m_elements; 
    ImVec2   m_pos = {0,0};
    ImGuiCond m_pos_cond = {ImGuiCond_Once};
    ImVec2   m_size = {0,0};
    ImGuiCond m_size_cond = {ImGuiCond_Once};
    uint32_t          m_no_name_element_seq = 1;
    bool    m_show_close = false;
    ImGuiWindowFlags m_win_flags;
  };

  class OverlaySameLine: public OverlayElement {
  public:
    OverlayElementType getType() const override { return OverlayElementType::SAMELINE; }
  };

  class OverlayChildFrame : public OverlayElement
  {

  };

class Overlay {
public:
  enum OverlayStatus
  {
    OS_ENABLED = 1,
    OS_DISABLED,
  };
  explicit Overlay( GLFWwindow* window );

  template <typename... Args>
  static OverlayPtr create(Args&&... args) 
  { 
    return std::make_unique<Overlay>(std::forward<Args>(args)...); 
  }
  /**
  @brief:xlm, as the seperator(sameline) element has no name, so can not get it by this function
  @param: 
  @return:
  */
  OverlayElement* getOverlayElementByName(const std::string& name);
  /**
  @brief:xlm, get the overlay status,
  @param: 
  @return: 1 enabled, 2 disabled
  */
  OverlayStatus getStatus();
  void setStatus(OverlayStatus status );
  template <typename T, typename... Args>
  T* addOverlayElement(const std::string& label, Args&&... args)
  {
    auto it = m_elements.find(label);
    if (it != m_elements.end())
    {
      delete it->second;
    }
    auto* elem = new T(label, std::forward<Args>(args)...); 
    if (elem)
      m_elements[label] = elem;
    return elem;
  }
  template <typename T, typename... Args>
  T* addNoNameOverlayElement( Args&&... args)
  {
    std::string label(typeid(T).name());
    label.append(std::to_string(m_no_name_element_seq));
    m_no_name_element_seq = (m_no_name_element_seq + 1)%0xffffffff;
    auto it = m_elements.find(label);
    if (it != m_elements.end())
    {
      delete it->second;
    }
    auto* elem = new T(std::forward<Args>(args)...); 
    if (elem)
      m_elements[label] = elem;
    return elem;
  }


  template <typename T>
  T* addOverlayElement(T* elem)
  {
    auto it = m_elements.find(elem->getLabel());
    if (it != m_elements.end())
    {
      delete it->second;
    }
    m_elements[elem->getLabel()] = elem;
    return elem;
  }


  bool hasKeyboardFocus() const;
  void render();

  ~Overlay();

private:
  //we offen wanna access the elemnt by name, so change the container from vector to map
  std::map<std::string, OverlayElement* > m_elements; 
  uint32_t          m_no_name_element_seq = 1;
  //added by xlm
  OverlayStatus       m_status; //
};

} // namespace Raz

#endif // RAZ_OVERLAY_HPP
