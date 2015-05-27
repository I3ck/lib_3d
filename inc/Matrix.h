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

    inline Matrix<T> operator * (const Matrix<T> &other) const {
        Matrix<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out.m[i][j] =
                    m[i][0] * other.m[0][j] +
				    m[i][1] * other.m[1][j] +
				    m[i][2] * other.m[2][j] +
				    m[i][3] * other.m[3][j];
            }
        }
        return out;
    }

    inline Matrix<T> operator * (T other) const {
        Matrix<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out.m[i][j] = other * m[i][j];
            }
        }
        return out;
    }

    inline friend Matrix<T> operator * (T other, Matrix<T> matrix) {
        Matrix<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out.m[i][j] = other * matrix.m[i][j];
            }
        }
        return out;
    }

//------------------------------------------------------------------------------

    bool equal_to(const Matrix<T> &other) const {
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                if(m[i][j] != other.m[i][j])
                    return false;
            }
        }
        return true;
    }

//------------------------------------------------------------------------------

    bool operator == (const Matrix<T> &other) const {
        return equal_to(other);
    }

    bool operator != (const Matrix<T> &other) const {
        return !equal_to(other);
    }
};

} // namespace lib_3d


#endif // MATRIX_H_INCLUDED
