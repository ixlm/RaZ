
#if 0
#include "RaZ/Utils/Gizmo.h"
namespace Raz
{
#define imguiGizmo_INTERPOLATE_NORMALS
static constexpr float STARTING_ALPHA_PLANE = 0.75f;

struct ImguiGizmo
{
    Quaternionf qtV = {Radiansf(0.0f), 0, 0, 1};  // Quaternion value
    Quaternionf qtV2 = {Radiansf(0.0f), 0, 0, 1}; // Quaternion value

    enum
    {                         // 0b0000'0000, //C++14 notation
        mode3Axes = 0x01,     // 0b0000'0001,
        modeDirection = 0x02, // 0b0000'0010,
        modeDirPlane = 0x04,  // 0b0000'0010,
        modeDual = 0x08,      // 0b0000'1000,
        modeMask = 0x0f,      // 0b0000'1111,

        cubeAtOrigin = 0x10,    // 0b0000'0000,
        sphereAtOrigin = 0x20,  // 0b0001'0000,
        noSolidAtOrigin = 0x40, // 0b0010'0000,
        modeFullAxes = 0x80,
        axesModeMask = 0xf0 // 0b1111'0000
    };

    enum
    {
        sphereTess16,
        sphereTess8,
        sphereTess4,
        sphereTess2
    };

    enum
    {
        CONE_SURF,
        CONE_CAP,
        CYL_SURF,
        CYL_CAP
    };

    // enum { SOLID_SURF, SOLID_CAP }
    // enum {
    enum
    {
        axisIsX,
        axisIsY,
        axisIsZ
    };

    enum solidSides
    {
        backSide,
        frontSide
    }; // or viceversa...

    // static ImVector<Vec3f> sphereVtx;
    static ImVector<Vec3f> sphereVtx;
    static ImVector<int> sphereTess;

    static ImVector<Vec3f> cubeVtx;
    static ImVector<Vec3f> cubeNorm;
    static ImVector<Vec3f> planeVtx;
    static ImVector<Vec3f> planeNorm;
    static ImVector<Vec3f> arrowVtx[4];
    static ImVector<Vec3f> arrowNorm[4];


    static void buildPlane(const float size,
                           const float thickness = planeThickness)
    {
        buildPolygon(Vec3f(thickness, size, size), planeVtx,
                     planeNorm);
    }

    static void buildCube(const float size)
    {
        buildPolygon(Vec3f(size, size, size), cubeVtx, cubeNorm);
    }

    static void buildPolygon(const Vec3f &size,
                             ImVector<Vec3f> &vtx,
                             ImVector<Vec3f> &norm);
    static void buildSphere(const float radius, const int tessFactor);
    static void buildCone(const float x0, const float x1, const float radius,
                          const int slices);
    static void buildCylinder(const float x0, const float x1,
                              const float radius, const int slices);

    // helper functions
    ///////////////////////////////////////
    static void resizeAxesOf(const Vec3f &newSize)
    {
        savedAxesResizeFactor = axesResizeFactor;
        axesResizeFactor = newSize;
    }

    static void restoreAxesSize() { axesResizeFactor = savedAxesResizeFactor; }

    static void resizeSolidOf(float newSize)
    {
        savedSolidResizeFactor = solidResizeFactor;
        solidResizeFactor = newSize;
    }

    static void restoreSolidSize()
    {
        solidResizeFactor = savedSolidResizeFactor;
    }

    static void setDirectionColor(ImU32 dColor, const ImU32 pColor)
    {
        setDirectionColor(ImGui::ColorConvertU32ToFloat4(dColor),
                          ImGui::ColorConvertU32ToFloat4(pColor));
    }

    static void setDirectionColor(const ImVec4 &dColor, const ImVec4 &pColor)
    {
        savedDirectionColor = directionColor;
        savedPlaneColor = planeColor;
        directionColor = dColor;
        planeColor = pColor;
    }

    static void setDirectionColor(ImU32 color)
    {
        setDirectionColor(ImGui::ColorConvertU32ToFloat4(color));
    }

    static void setDirectionColor(const ImVec4 &color)
    {
        setDirectionColor(
            color, ImVec4(color.x, color.y, color.z, STARTING_ALPHA_PLANE));
    }

    static void restoreDirectionColor()
    {
        directionColor = savedDirectionColor;
        planeColor = savedPlaneColor;
    }

    static void setSphereColors(const ImVec4 &a, const ImVec4 &b)
    {
        setSphereColors(ImGui::ColorConvertFloat4ToU32(a),
                        ImGui::ColorConvertFloat4ToU32(b));
    }

    static void setSphereColors(ImU32 a, ImU32 b)
    {
        savedSphereColors[0] = sphereColors[0];
        savedSphereColors[1] = sphereColors[1];
        sphereColors[0] = a;
        sphereColors[1] = b;
    }

    static void restoreSphereColors()
    {
        sphereColors[0] = savedSphereColors[0];
        sphereColors[1] = savedSphereColors[1];
    }

    //
    //  internals
    //
    ////////////////////////////////////////////////////////////////////////////
    static bool solidAreBuilded;
    static bool dragActivate;

    int drawMode = mode3Axes;
    int axesOriginType = cubeAtOrigin;
    bool showFullAxes = false;

    bool drawFunc(const char *label, float size);

    void modeSettings(int mode)
    {
        drawMode = int(mode & modeMask);
        axesOriginType = int(mode & axesModeMask);
        showFullAxes = bool(modeFullAxes & mode);
    }

    void setDualMode(const int mode)
    {
        modeSettings((ImguiGizmo::modeDual | ImguiGizmo::axesModeMask) &
                     (mode | ImguiGizmo::modeDual));
    }

    // vec3 -> quat -> trackbalTransforms -> quat -> vec3
    ////////////////////////////////////////////////////////////////////////////
    bool getTransforms(Quaternionf &q, const char *label,
                       Vec3f &dir, float size)
    {
        // const float len = Diligent::length(dir);
        const float len = dir.computeLength();
        // q = Quaternionf::RotationFromAxisAngle(
        //     Diligent::normalize(Vec3f(0, -dir.z, dir.y)),
        //     acosf(dir.x / len));
        Vec3f normal = Vec3f(0.0f, -dir[2], dir[1]).normalize();
        q = Quaternionf(Radiansf(acosf(dir[0]/len)), normal);

        bool ret = drawFunc(label, size);
        if (ret)
            dir = q.RotateVector(Vec3f(1, 0, 0)) *
                  len; // return vector with original length

        return ret;
    }
    // Vec4 (xyz axis, w angle) -> quat -> trackbalTransforms -> quat -> vec4
    ////////////////////////////////////////////////////////////////////////////
    bool getTransforms(Quaternionf &q, const char *label,
                       Diligent::float4 &axis_angle, float size)
    {
        q = Quaternionf::RotationFromAxisAngle(
            Vec3f(axis_angle),
            axis_angle.w); // g.ConvertFromAxisAngle();

        bool ret = drawFunc(label, size);

        if (ret)
            q.GetAxisAngle((Vec3f &)axis_angle, axis_angle.w);

        return ret;
    }

    //
    //  Settings
    //
    //      axes/arrow are composed of cone (or pyramid) and cylinder
    //      (or parallelepiped): this solid are builded at first instance
    //      and will have same slices/radius/length for all controls in your
    //      applications but can be  resized proportionally with a reductin
    //      factor: resizeAxesOf and resizeSolidOf (below).
    //      Also the colors of sphere tessellation are set at buil time,
    //      while colors of axes and cube are fixed
    //
    //      if you want change solids attributes, change the static variables.
    //      If you need to resize solid and axes use resizeAxesOf and
    //      resizeSolidOf, they works like push/pop stack (without buffer!)
    //      with respective restoreAxesSize and restoreSolidSize.
    //      for example:
    //          // reDim axes ... same lenght,
    //          imguiGizmo::resizeAxesOf(Vec3f(imguiGizmo::axesResizeFactor.x,
    //          2.0, 2.0)); imguiGizmo::resizeSolidOf(1.25); // sphere bigger
    //          ImGui::gizmo3D("##RotB", b,sz);
    //          imguiGizmo::restoreSolidSize(); // restore at default
    //          imguiGizmo::restoreAxesSize();
    ////////////////////////////////////////////////////////////////////////////

    //
    //  Build solid components
    //
    ////////////////////////////////////////////////////////////////////////////

    // arrow/axes components
    ///////////////////////////////////////
    static int coneSlices;
    static float coneRadius;
    static float coneLength;

    static int cylSlices;
    static float cylRadius; // sizeCylLength ... defined in base to control size

    // Sphere components
    ///////////////////////////////////////
    static float sphereRadius;
    static int sphereTessFactor;

    // Cube components
    ///////////////////////////////////////
    static float cubeSize;

    // Plane components
    ///////////////////////////////////////
    static float planeSize;
    static float planeThickness;

    //
    //  Resizing and color settings
    //
    ////////////////////////////////////////////////////////////////////////////

    // Axes reduction
    ///////////////////////////////////////
    static Vec3f axesResizeFactor;
    static Vec3f savedAxesResizeFactor;

    // solid reduction
    ///////////////////////////////////////
    static float solidResizeFactor;
    static float savedSolidResizeFactor;

    static ImU32 sphereColors[2]; // Tessellation colors
    static ImU32 savedSphereColors[2];
    // ImU32 spherecolorA=0xff005cc0, spherecolorB=0xffc05c00;

    static ImVec4 directionColor;
    static ImVec4 savedDirectionColor;

    static ImVec4 planeColor;
    static ImVec4 savedPlaneColor;

    static const int imguiGizmoDefaultSize;
};

OverlayGizmo::OverlayGizmo(const std::string &name, Quaternionf &quat,
                           float size, const uint32_t flag)
    : OverlayElement(std::move(name))
{
}
OverlayGizmo::OverlayGizmo(const std::string &name, Quaternionf &axes,
                           Quaternionf &spot, float size, const uint32_t flag)
    : OverlayElement(std::move(name))
{
}
OverlayGizmo::OverlayGizmo(const std::string &name, Quaternionf &axes,
                           Vec3f &spot_dir, float size, const uint32_t flag)
    : OverlayElement(std::move(name))
{
}
OverlayGizmo::OverlayGizmo(const std::string &name, Quaternionf &axes,
                           Vec4f &axes_angle, float size, const uint32_t flag)
    : OverlayElement(std::move(name))
{
}
OverlayGizmo::OverlayGizmo(const std::string &name, Vec4f &axis_angle,
                           float size, const uint32_t flag)
    : OverlayElement(std::move(name))
{
}
OverlayGizmo::OverlayGizmo(const std::string &name, Vec3f &dir, float size,
                           const uint32_t flag)
    : OverlayElement(std::move(name))
{
}

void OverlayGizmo::render() {}

} // namespace Raz
#endif