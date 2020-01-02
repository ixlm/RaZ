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
};


  class OverlayElement;
  // using OverlayElementPtr = std::unique_ptr<OverlayElement>;

  class OverlayLabel;
  // using OverlayLabelPtr = std::unique_ptr<OverlayLabel>;

  class OverlayButton;
  // using OverlayButtonPtr = std::unique_ptr<OverlayButton>;

  class OverlayCheckbox;
  // using OverlayCheckboxPtr = std::unique_ptr<OverlayCheckbox>;

  class OverlayTextbox;
  // using OverlayTextboxPtr = std::unique_ptr<OverlayTextbox>;

  class OverlaySeparator;
  // using OverlaySeparatorPtr = std::unique_ptr<OverlaySeparator>;

  class OverlayFrameTime;
  // using OverlayFrameTimePtr = std::unique_ptr<OverlayFrameTime>;

  class OverlayFpsCounter;
  // using OverlayFpsCounterPtr = std::unique_ptr<OverlayFpsCounter>;
  class OverlayFrame;
  class OverlaySameLine;
  // using OverlayFramePtr = std::unique_ptr<OverlayFrame>;


    // template <typename T, typename... Args>
    // T* createOverlayElement(Args&&... args) 
    // {
    //   return new T(std::forward<Args>(args)...);
    // }



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

    // template <typename... Args>
    // static OverlayLabelPtr create(Args&&... args) { return std::make_unique<OverlayLabel>(std::forward<Args>(args)...); }
    // template <typename... Args>
    // static OverlayLabel* create(Args&&... args) 
    // { 
    //   // return std::make_unique<OverlayLabel>(std::forward<Args>(args)...); 
    //   return new OverlayLabel(std::forward<Args>(args)...);
    // }
  };

  class OverlayButton : public OverlayElement {
    friend Overlay;
    friend OverlayFrame;

  public:
    OverlayButton(std::string label, std::function<void()> actionClick) : OverlayElement(std::move(label)), m_actionClick{ std::move(actionClick) } {}

    OverlayElementType getType() const override { return OverlayElementType::BUTTON; }

    // template <typename... Args>
    // static OverlayButtonPtr create(Args&&... args) { return std::make_unique<OverlayButton>(std::forward<Args>(args)...); }
    // template <typename... Args>
    // static OverlayButton* create(Args&&... args) 
    // { 
    //   //return std::make_unique<OverlayButton>(std::forward<Args>(args)...); 
    //   return new OverlayButton(std::forward<Args>(args)...);
    // }

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

    // template <typename... Args>
    // static OverlayCheckboxPtr create(Args&&... args) { return std::make_unique<OverlayCheckbox>(std::forward<Args>(args)...); }

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

    // template <typename... Args>
    // static OverlayTextboxPtr create(Args&&... args) { return std::make_unique<OverlayTextbox>(std::forward<Args>(args)...); }

  private:
    std::string m_text {};
    std::function<void(const std::string&)> m_callback {};
  };

  class OverlaySeparator : public OverlayElement {
  public:
    OverlayElementType getType() const override { return OverlayElementType::SEPARATOR; }

    // template <typename... Args>
    // static OverlaySeparatorPtr create(Args&&... args) { return std::make_unique<OverlaySeparator>(std::forward<Args>(args)...); }
  };

  class OverlayFrameTime : public OverlayElement {
  public:
    explicit OverlayFrameTime(std::string formattedLabel) : OverlayElement(std::move(formattedLabel)) {}

    OverlayElementType getType() const override { return OverlayElementType::FRAME_TIME; }

    // template <typename... Args>
    // static OverlayFrameTimePtr create(Args&&... args) { return std::make_unique<OverlayFrameTime>(std::forward<Args>(args)...); }
  };

  class OverlayFpsCounter : public OverlayElement {
  public:
    explicit OverlayFpsCounter(std::string formattedLabel) : OverlayElement(std::move(formattedLabel)) {}

    OverlayElementType getType() const override { return OverlayElementType::FPS_COUNTER; }

    // template <typename... Args>
    // static OverlayFpsCounterPtr create(Args&&... args) { return std::make_unique<OverlayFpsCounter>(std::forward<Args>(args)...); }
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
    // template <typename... Args>
    // static OverlayFramePtr create(Args&&... args) 
    // {
    //   return std::make_unique<OverlayFrame>(std::forward<Args>(args)...);
    // }

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
    // std::vector<OverlayElementPtr> m_elements;
    ImVec2   m_pos = {0,0};
    ImGuiCond m_pos_cond = {ImGuiCond_Once};
    ImVec2   m_size = {0,0};
    ImGuiCond m_size_cond = {ImGuiCond_Once};
    uint32_t          m_no_name_element_seq = 1;
    bool    m_show_close = false;
    ImGuiWindowFlags m_win_flags;

    // added by xlm
    // std::string   m_name; //the name of overlay, we can get it by name
  };

  class OverlaySameLine: public OverlayElement {
  public:
    OverlayElementType getType() const override { return OverlayElementType::SAMELINE; }

    // template <typename... Args>
    // static OverlaySeparatorPtr create(Args&&... args) { return std::make_unique<OverlaySeparator>(std::forward<Args>(args)...); }
  };

class Overlay {
public:
  enum OverlayStatus
  {
    OS_ENABLED = 1,
    OS_DISABLED,
  };
  // explicit Overlay(GLFWwindow* window);
  explicit Overlay( GLFWwindow* window );

  template <typename... Args>
  static OverlayPtr create(Args&&... args) 
  { 
    return std::make_unique<Overlay>(std::forward<Args>(args)...); 
  }
  // template <typename... Args>
  // static Overlay* create(Args&&... args) 
  // { 
  //   // return std::make_unique<Overlay>(std::forward<Args>(args)...); 
  //   return new Overlay(std::forward<Args>(args)...);
  // }

  // template <typename... Args>
  // static Overlay* create(const std::string& name, Args&&... args) 
  // { 
  //   // return std::make_unique<Overlay>(std::forward<Args>(args)...); 
  //   return new Overlay(name, std::forward<Args>(args)...);
  // }

  // void setName(const std::string& name);
  // std::string& getName();
  /**
  @brief:xlm, get the overlay status,
  @param: 
  @return: 1 enabled, 2 disabled
  */
  OverlayStatus getStatus();
  void setStatus(OverlayStatus status );

  // void addLabel(std::string label);
  // void addButton(std::string label, std::function<void()> actionClick);
  // void addCheckbox(std::string label, std::function<void()> actionOn, std::function<void()> actionOff, bool initVal);
  // void addTextbox(std::string label, std::function<void(const std::string&)> callback);
  // void addSeparator();
  // void addFrameTime(std::string formattedLabel);
  // void addFpsCounter(std::string formattedLabel);

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

  // std::vector<OverlayElementPtr> m_elements;
  //we offen wanna to access the elemnt by name, so change the container from vector to map
  std::map<std::string, OverlayElement* > m_elements; 
  uint32_t          m_no_name_element_seq = 1;

  //added by xlm
  // std::string   m_name; //the name of overlay, we can get it by name
  OverlayStatus       m_status; //
};

} // namespace Raz

#endif // RAZ_OVERLAY_HPP
