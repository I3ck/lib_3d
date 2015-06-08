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
        const T length = this->length();

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

    Vec<T> cross(const Vec<T> &other) const {
        Vec<T> out;

        out.x = this->y * other.z - this->z * other.y;
        out.y = this->z * other.x - this->x * other.z;
        out.z = this->x * other.y - this->y * other.x;

        return out;
    }

//------------------------------------------------------------------------------

    T dot(const Vec<T> &other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

//------------------------------------------------------------------------------

    inline Vec<T> operator * (const Matrix<T> &m) const {
        Vec<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * (*this)[j];
            }
        }
        return out;
    }

    inline Vec<T>& operator * (const Matrix<T> &m) {
        *this = *this * m;
        return *this;
    }
};

} // namespace lib_3d


#endif // VEC_H_INCLUDED
