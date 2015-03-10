#pragma once

#include <cmath>
#include <limits>

namespace Common
{

template <typename T, typename U>
class Eql;

template <typename T>
class Eql<T, T>
{
public:
    static bool f(const T& t, const T& u)
    {
        return std::fabs(t - u) < eps;
    }

    static constexpr T eps = std::numeric_limits<T>::epsilon();
};

template <typename T>
constexpr T Eql<T, T>::eps;

template <typename T, typename U>
inline bool eql(const T& t, const U& u)
{
    return Eql<T, U>::f(t, u);
}

}
