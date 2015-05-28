/**
 * \file    MatrixFactory.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef MATRIX_FACTORY_H_INCLUDED
#define MATRIX_FACTORY_H_INCLUDED

#include "Matrix.h"

namespace lib_3d {

template <typename T>
class MatrixFactory {
public:
    ///@todo overloads for Point or maybe later Vector
    ///or add proper methods to Point / Vector to easily achieve this?

    static Matrix<T> translation(T x, T y, T z) {
        Matrix<T> out;
        out.m[0][0] = 1.0;  out.m[0][1] = 0.0;  out.m[0][2] = 0.0; out.m[0][3] = x;
        out.m[1][0] = 0.0;  out.m[1][1] = 1.0;  out.m[1][2] = 0.0; out.m[1][3] = y;
        out.m[2][0] = 0.0;  out.m[2][1] = 0.0;  out.m[2][2] = 1.0; out.m[2][3] = z;
        out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0; out.m[3][3] = 1.0;
        return out;
    }

};

} // namespace lib_3d


#endif // MATRIX_FACTORY_H_INCLUDED
