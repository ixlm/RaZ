#pragma once

#ifndef RAZ_CONSTANTS_HPP
#define RAZ_CONSTANTS_HPP

namespace Raz {

template <typename T>
// constexpr T PI = static_cast<T>(3.14159265358979323846L);
constexpr T PI = static_cast<T>(3.1415926535897932384626433833L);
template <typename T>
constexpr T MATH_E = static_cast<T>(2.7182818284590452353602874714L);

} // namespace Raz

#endif // RAZ_CONSTANTS_HPP
