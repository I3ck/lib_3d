/**
 * \file    Vec.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#include "PointVec.h"

namespace lib_3d {

template <typename T>
class Vec : public PointVec<T> {
public:

    Vec(T x=0.0, T y=0.0, T z=0.0) :
        PointVec<T>(x, y, z)
    {
    }

//------------------------------------------------------------------------------

    ~Vec() {

    }

//------------------------------------------------------------------------------

    Vec<T>& normalize() {
        const T length = length();

        this->x /= length;
        this->y /= length;
        this->z /= length;

        return *this;
    }

//------------------------------------------------------------------------------

    T length() const {
        return sqrt( pow(this->x,2) + pow(this->y,2) + pow(this->z,2) );
    }

//------------------------------------------------------------------------------

    inline Vec<T> operator * (const Matrix<T> &m) const {
        Vec<T> copy = *this;
        Vec<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * copy[j];
            }
        }
        return out;
    }
};

} // namespace lib_3d


#endif // VEC_H_INCLUDED
