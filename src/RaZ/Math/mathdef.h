#ifndef __RAZ_MATH_DEF_H___
#define __RAZ_MATH_DEF_H__

#include "RaZ/Math/Angle.hpp"
#include "mathfu/vector.h"
#include "mathfu/constants.h"
#include "mathfu/io.h"
#include "mathfu/matrix.h"
#include "mathfu/quaternion.h"
#include "mathfu/rect.h"
#include "mathfu/utilities.h"
// #include "mathfu/glsl_mappings.h"
// #include "mathfu/hlsl_mappings.h"
#include <initializer_list>
using namespace mathfu;
//----------------------vector--------------------------
// template <typename T> using Vec2 = mathfu::Vector<T, 2>;
// template <typename T> using Vec3 = mathfu::Vector<T, 3>;
// template <typename T> using Vec4 = mathfu::Vector<T, 4>;

// using Vec2b = Vec2<uint8_t>;
// using Vec3b = Vec3<uint8_t>;
// using Vec4b = Vec4<uint8_t>;

// using Vec2i = Vec2<int>;
// using Vec3i = Vec3<int>;
// using Vec4i = Vec4<int>;

// using Vec2ul = Vec2<uint32_t>;
// using Vec3ul = Vec3<uint32_t>;
// using Vec4ul = Vec4<uint32_t>;

// using Vec2f = Vec2<float>;
// using Vec3f = Vec3<float>;
// using Vec4f = Vec4<float>;

// using Vec2d = Vec2<double>;
// using Vec3d = Vec3<double>;
// using Vec4d = Vec4<double>;

// namespace Axis {

// const Vec3f X({ 1.f, 0.f, 0.f });
// const Vec3f Y({ 0.f, 1.f, 0.f });
// const Vec3f Z({ 0.f, 0.f, 1.f });

// }

//-----------------------matrix--------------------------------
// template <typename T> using Mat2 = mathfu::Matrix<T, 2, 2>;
// template <typename T> using Mat3 = mathfu::Matrix<T, 3, 3>;
// template <typename T> using Mat4 = mathfu::Matrix<T, 4, 4>;

// using Mat2b = Mat2<uint8_t>;
// using Mat3b = Mat3<uint8_t>;
// using Mat4b = Mat4<uint8_t>;

// using Mat2i = Mat2<int>;
// using Mat3i = Mat3<int>;
// using Mat4i = Mat4<int>;

// using Mat2ul = Mat2<unsigned long>;
// using Mat3ul = Mat3<unsigned long>;
// using Mat4ul = Mat4<unsigned long>;

// using Mat2f = Mat2<float>;
// using Mat3f = Mat3<float>;
// using Mat4f = Mat4<float>;

// using Mat2d = Mat2<double>;
// using Mat3d = Mat3<double>;
// using Mat4d = Mat4<double>;

//--------------------quaternion---------------------------------
// using Quaternionf = mathfu::Quaternion<float>;
// using Quaterniond = mathfu::Quaternion<double>;


// template <typename T>
// static bool isInsideTriangle(const Vec3<T> &a, const Vec3<T> &b,
//                              const Vec3<T> &c, const Vec3<T> &p, bool include_edges = true)
// {
//     auto ab = b -a ;
//     auto ap = p -a;
//     auto cross_abp = ab.dot(ap);

//     auto bc = c - b; 
//     auto bp = p - b;
//     auto cross_bcp = bc.dot(bp);

//     auto ca = a - c;
//     auto cp = p - c;
//     auto cross_cap = ca.dot(cp);

//     //if in the triangle, for edge ab,ap, the angle between ab and ap is a acute angle,so the dot=||ab||*||ap||*cos(angle) > 0, same to the other edges

//     if (include_edges) //if in the edge, the dot result will be 0
//         return (cross_abp > 0) && (cross_bcp > 0) && (cross_cap > 0);
//     else
//         return (cross_abp >= 0) && (cross_bcp >= 0) && (cross_cap >= 0); 
// }

// template <typename T>
// T infinity()
// {
//     return std::numeric_limits<T>::infinity();
// }

// template <typename T>
// inline T eps(T a, T b)
// {
//     const T aa = abs(a) = 1.0;
//     if (aa == infinity<T>())
//         return EPSILION;
//     else
//         return EPSILION * aa;
// }


#endif