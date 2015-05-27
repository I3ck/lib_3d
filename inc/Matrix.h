/**
 * \file    Matrix.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <sstream>
#include <cmath>

namespace lib_3d {

template <typename T>
class Matrix {

public:
    T m[4][4];

//------------------------------------------------------------------------------

    Matrix() {
        m[0][0] = 1.0;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = 0.0;
        m[1][0] = 0.0;  m[1][1] = 1.0;  m[1][2] = 0.0;  m[1][3] = 0.0;
        m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] = 1.0;  m[2][3] = 0.0;
        m[3][0] = 0.0;  m[3][1] = 0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;
    }

//------------------------------------------------------------------------------

    ~Matrix() {

    }

//------------------------------------------------------------------------------
};

} // namespace lib_3d


#endif // MATRIX_H_INCLUDED
