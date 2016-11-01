/**
 * \file    Point.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "PointVec.h"
#include "Vec.h"

#include <functional>

namespace lib_3d {

template <typename T>
class Point : public PointVec<T> {


public:

    typedef T value_type; ///@todo move directly to PointVec

    Point(T x=0.0, T y=0.0, T z=0.0) :
        PointVec<T>(x, y, z)
    {}

    Point(PointVec<T> const& in) :
        PointVec<T>(in.x, in.y, in.z)
    {}

//------------------------------------------------------------------------------

    ~Point() {

    }

//------------------------------------------------------------------------------

    Point center_between(Point other) const {
        return Point{(T)(this->x + (other.x - this->x) / 2.0),
                     (T)(this->y + (other.y - this->y) / 2.0),
                     (T)(this->z + (other.z - this->z) / 2.0)};
    }

//------------------------------------------------------------------------------

    inline Vec<T> operator - (const PointVec<T> &other) const {
        return Vec<T>{this->x - other.x,
                      this->y - other.y,
                      this->z - other.z};
    }

    inline Point<T>& operator -= (const PointVec<T> &other) {
        auto v = *this - other;
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    inline Point<T> operator + (const PointVec<T> &vec) const {
        return Point<T>{this->x + vec.x,
                        this->y + vec.y,
                        this->z + vec.z};
    }

    inline Point<T>& operator += (const PointVec<T> &vec) {
        *this = *this + vec;
        return *this;
    }

    inline Point<T> operator * (const Matrix<T> &m) const {
        Point<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * (*this)[j];
            }
        }
        return out;
    }

    inline Point<T>& operator *= (const Matrix<T> &m) {
        *this = *this * m;
        return *this;
    }

};

///@todo seperate file?
template <typename T>
struct PointHasher
{
  std::size_t operator()(const Point<T>& p) const
  {
    return std::hash<T>()((T)(3.0 * p.x + 7.0 * p.y + 11.0 * p.z));
  }
};


} // namespace lib_3d


#endif // POINT_H_INCLUDED
