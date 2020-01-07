#ifndef __MATH_FUNC_HPP__
#define __MATH_FUNC_HPP__

#include "Constants.hpp"
#include "Vector.hpp"

namespace Raz
{
    //@brief, if a point p is in the triangle 
template <typename T>
static bool isInsideTriangle(const Vec3<T> &a, const Vec3<T> &b,
                             const Vec3<T> &c, const Vec3<T> &p, bool include_edges = true)
{
    auto ab = b -a ;
    auto ap = p -a;
    auto cross_abp = ab.dot(ap);

    auto bc = c - b; 
    auto bp = p - b;
    auto cross_bcp = bc.dot(bp);

    auto ca = a - c;
    auto cp = p - c;
    auto cross_cap = ca.dot(cp);

    //if in the triangle, for edge ab,ap, the angle between ab and ap is a acute angle,so the dot=||ab||*||ap||*cos(angle) > 0, same to the other edges

    if (include_edges) //if in the edge, the dot result will be 0
        return (cross_abp > 0) && (cross_bcp > 0) && (cross_cap > 0);
    else
        return (cross_abp >= 0) && (cross_bcp >= 0) && (cross_cap >= 0); 
}
}

#endif