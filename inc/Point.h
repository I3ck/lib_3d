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

namespace lib_3d {

template <typename T>
class Point : public PointVec<T> {
public:

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

    inline Point<T> operator * (const Matrix<T> &m) const {
        Point<T> copy = *this;
        Point<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * copy[j];
            }
        }
        return out;
    }
};

} // namespace lib_3d


#endif // POINT_H_INCLUDED
