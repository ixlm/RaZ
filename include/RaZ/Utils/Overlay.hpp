#pragma once

#ifndef RAZ_OVERLAY_HPP
#define RAZ_OVERLAY_HPP

#include "imgui/imgui.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
// #include <unordered_map>
#include <map>

// Forward declaration of GLFWwindow, to allow its usage into functions
struct GLFWwindow;

namespace Raz
{

extern uint64_t g_no_name_element_seq;



class Overlay;
using OverlayPtr = std::unique_ptr<Overlay>;

enum class OverlayElementType
{
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
  COMBOBOX,
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
template <typename T>
class OverlayCombobox;


// typedef std::function<void(OverlayElement*)> OverlayRenderer;
// void registerOverlayRender(OverlayElementType type, OverlayRenderer renderer);
// void unregisterOverlayRender(OverlayElementType type);

class OverlayElement
{
  friend Overlay;

public:
  OverlayElement() = default;
  explicit OverlayElement(const std::string &label) : m_label{std::move(label)}
  {
  }

  virtual OverlayElementType getType() const = 0;
  // static void registerRender();

  virtual ~OverlayElement() = default;
  virtual const std::string &getName() const { return m_label; }
  virtual void render() = 0;

protected:
  std::string m_label{};
};

class OverlayLabel : public OverlayElement
{
public:
  explicit OverlayLabel(const std::string &label)
      : OverlayElement(std::move(label))
  {
  }

  // static void registerRender();

  OverlayElementType getType() const override
  {
    return OverlayElementType::LABEL;
  }

  virtual void render() ;
};

class OverlayButton : public OverlayElement
{
public:
  OverlayButton(const std::string &label, std::function<void()> actionClick)
      : OverlayElement(std::move(label)), m_actionClick{std::move(actionClick)}
  {
  }

  OverlayElementType getType() const override
  {
    return OverlayElementType::BUTTON;
  }
  // static void registerRender();
  virtual void render() ;
private:
  std::function<void()> m_actionClick{};
};

class OverlayCheckbox : public OverlayElement
{
public:
  OverlayCheckbox(const std::string &label, std::function<void()> actionOn,
                  std::function<void()> actionOff, bool initVal)
      : OverlayElement(std::move(label)), m_actionOn{std::move(actionOn)},
        m_actionOff{std::move(actionOff)}, m_isChecked{initVal}
  {
  }

  OverlayElementType getType() const override
  {
    return OverlayElementType::CHECKBOX;
  }
  // static void registerRender();
  virtual void render() ;
private:
  std::function<void()> m_actionOn{};
  std::function<void()> m_actionOff{};
  bool m_isChecked{};
};

class OverlayTextbox : public OverlayElement
{
public:
  OverlayTextbox(const std::string &label,
                 std::function<void(const std::string &)> callback)
      : OverlayElement(std::move(label)), m_callback{std::move(callback)}
  {
  }

  OverlayElementType getType() const override
  {
    return OverlayElementType::TEXTBOX;
  }
  // static void registerRender();
  virtual void render() ;

private:
  std::string m_text{};
  std::function<void(const std::string &)> m_callback{};
};

class OverlaySeparator : public OverlayElement
{
public:
  OverlayElementType getType() const override
  {
    return OverlayElementType::SEPARATOR;
  }
  // static void registerRender();
  virtual void render() ;
};

class OverlayFrameTime : public OverlayElement
{
public:
  explicit OverlayFrameTime(const std::string &formattedLabel)
      : OverlayElement(std::move(formattedLabel))
  {
  }

  OverlayElementType getType() const override
  {
    return OverlayElementType::FRAME_TIME;
  }
  // static void registerRender();
  virtual void render() ;
};

class OverlayFpsCounter : public OverlayElement
{
public:
  explicit OverlayFpsCounter(const std::string &formattedLabel)
      : OverlayElement(std::move(formattedLabel))
  {
  }

  OverlayElementType getType() const override
  {
    return OverlayElementType::FPS_COUNTER;
  }
  // static void registerRender();
  virtual void render() ;
};

class ContainerInterface
{
public:
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
    label.append(std::to_string(g_no_name_element_seq));
    g_no_name_element_seq = (g_no_name_element_seq + 1) % 0xffffffff;
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

protected:
  std::map<std::string, OverlayElement *> m_elements;
};

// added by xlm
class OverlayFrame : public OverlayElement, public ContainerInterface
{
public:
  friend Overlay;
  explicit OverlayFrame(const std::string &name, bool show_close,
                        ImGuiWindowFlags flags)
      : OverlayElement(std::move(name)), m_show_close(show_close),
        m_win_flags(flags)
  {
  }
  OverlayElementType getType() const override
  {
    return OverlayElementType::FRAME;
  }
  // static void registerRender();

  void setFramePos(const ImVec2 &pos, ImGuiCond cond)
  {
    m_pos = pos;
    m_pos_cond = cond;
  }
  void setFrameSize(const ImVec2 &size, ImGuiCond cond)
  {
    m_size = size;
    m_size_cond = cond;
  }
  void render();

private:
  // std::map<std::string, OverlayElement* > m_elements;
  ImVec2 m_pos = {0, 0};
  ImGuiCond m_pos_cond = {ImGuiCond_Once};
  ImVec2 m_size = {0, 0};
  ImGuiCond m_size_cond = {ImGuiCond_Once};
  // uint32_t          m_no_name_element_seq = 1;
  bool m_show_close = false;
  ImGuiWindowFlags m_win_flags;
};

class OverlaySameLine : public OverlayElement
{
public:
  OverlayElementType getType() const override
  {
    return OverlayElementType::SAMELINE;
  }
  // static void registerRender();
  virtual void render() ;
};

class OverlayChildFrame : public OverlayElement, public ContainerInterface
{
public:
  explicit OverlayChildFrame(const std::string &name, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
      : OverlayElement(std::move(name))
      , m_size(size_arg), m_has_boder(border), m_win_flags(flags)
  {
  }
  OverlayElementType getType() const override
  {
    return OverlayElementType::CHILD_FRAME;
  }
  void render();
  // static void registerRender();
protected:
  ImVec2  m_size = {0,0};
  bool    m_has_boder = false;
  ImGuiWindowFlags m_win_flags ;

};

template <typename T>
class OverlayCombobox : public OverlayElement
{
public:
  explicit OverlayCombobox(const std::string& name, int32_t curr_idx = 0, const std::vector<  std::pair<T, const char* > >& item_vec  = nullptr, int32_t popup_max_height_in_items = -1)
  : OverlayElement(std::move(name))
  , m_max_popup_height(popup_max_height_in_items)
  , m_curr_idx(curr_idx)
  {
      for (int i = 0; i < item_vec.size(); i++)
      {
        m_items.push_back(item_vec[i]);
      }
  }
  OverlayElementType getType() const override
  {
    return OverlayElementType::COMBOBOX;
  }
  virtual void render() 
  {
    int item_count = m_items.size();
    std::vector<const char*> names(item_count);
    for (int i=0; i<m_items.size(); i++)
      names[i] = m_items[i].second;
    ImGui::Combo(m_label.c_str(), &m_curr_idx, names.data(), item_count, m_max_popup_height);
  }
protected:
  std::vector< std::pair<T, const char* > > m_items;
  int32_t m_max_popup_height;
  int32_t m_curr_idx = 0;
};

class Overlay : public ContainerInterface
{
public:
  enum OverlayStatus
  {
    OS_ENABLED = 1,
    OS_DISABLED,
  };
  explicit Overlay(GLFWwindow *window);

  template <typename... Args> static OverlayPtr create(Args &&... args)
  {
    return std::make_unique<Overlay>(std::forward<Args>(args)...);
  }
  /**
  @brief:xlm, as the seperator(sameline) element has no name, so can not get it
  by this function
  @param:
  @return:
  */
  OverlayElement *getOverlayElementByName(const std::string &name);
  /**
  @brief:xlm, get the overlay status,
  @param:
  @return: 1 enabled, 2 disabled
  */
  OverlayStatus getStatus();
  void setStatus(OverlayStatus status);


  bool hasKeyboardFocus() const;
  void render();

  ~Overlay();

private:
  OverlayStatus m_status; //
};

} // namespace Raz

#endif // RAZ_OVERLAY_HPP
