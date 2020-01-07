#ifndef __GIZMO_H__
#define __GIZMO_H__
#include "Overlay.hpp"
// #include "Math/Quaternion.hpp"
// #include "Math/Vector.hpp"

namespace Raz
{
    #if 0

class OverlayGizmo : public OverlayElement
{
public:

  explicit OverlayGizmo(const std::string& name, Quaternionf& quat, float size, const uint32_t flag);
  explicit OverlayGizmo(const std::string& name, Quaternionf& axes, Quaternionf& spot, float size, const uint32_t flag);
  explicit OverlayGizmo(const std::string& name, Quaternionf& axes, Vec3f& spot_dir, float size, const uint32_t flag);
  explicit OverlayGizmo(const std::string& name, Quaternionf& axes, Vec4f& axes_angle, float size, const uint32_t flag);
  explicit OverlayGizmo(const std::string& name, Vec4f& axis_angle, float size, const uint32_t flag);
  explicit OverlayGizmo(const std::string& name, Vec3f& dir, float size, const uint32_t flag);

  virtual const char* getType() const 
  {
      return "OverlayGizmo";
  }
  virtual void render() ;
protected:
    float       m_size;
    uint32_t    m_flag; 
    uint8_t     m_type;

    union GizmoParam
    {
        struct type1
        {
            Quaternionf quat;
        };
        struct type2
        {
            Quaternionf axes;
            Quaternionf spot;
        };
        struct type3
        {
            Quaternionf axes;
            Vec3f spot_dir;
        };
        struct type4
        {
            Quaternionf axes;
            Vec4f axes_angle;
        };
        struct type5
        {
            Vec4f axis_angle;
        };
        struct type6
        {
            Vec3f dir;
        };
    };
    GizmoParam m_param;

};
#endif
    
} // namespace Raz


#endif