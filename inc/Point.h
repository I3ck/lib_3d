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

namespace lib_3d {

template <typename T>
class Point : public PointVec<T> {


public:

    typedef T value_type; ///@todo move directly to PointVec

    Point(T x=0.0, T y=0.0, T z=0.0) :
        PointVec<T>(x, y, z)
    {
    }

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

    inline Vec<T> operator - (const Point<T> &other) const {
        return Vec<T>{this->x - other.x,
                      this->y - other.y,
                      this->z - other.z};
    }

    inline Point<T> operator + (const Vec<T> &vec) const {
        return Point<T>{this->x + vec.x,
                        this->y + vec.y,
                        this->z + vec.z};
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
};

} // namespace lib_3d


#endif // POINT_H_INCLUDED
