/*
  Copyright 2016 Martin Buck
  This file is part of lib_3d.
  lib_3d is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  lib_3d is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License
  along with lib_3d.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <sstream>
#include <cmath>
#include <sstream>
#include <array>

namespace lib_3d {

template <typename T>
class Matrix {
    using matrix = std::array<std::array<T,4>,4>;

public:
    matrix m;

//------------------------------------------------------------------------------

    Matrix() :
        m{{ {1.0, 0.0, 0.0, 0.0}
         ,  {0.0, 1.0, 0.0, 0.0}
         ,  {0.0, 0.0, 1.0, 0.0}
         ,  {0.0, 0.0, 0.0, 1.0}
         }}
    {}

    Matrix(Matrix const&) = default;
    Matrix(Matrix &&) = default;
    Matrix& operator = (Matrix const&) = default;
    Matrix& operator = (Matrix &&) = default;
    ~Matrix() = default;

    Matrix(matrix const& in) :
        m(in)
    {}

    Matrix(matrix && in) :
        m(std::move(in))
    {}

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
        std::stringstream out;
        out << m[0][0] << divider << m[0][1] << divider << m[0][2] << divider << m[0][3] << "\n"
            << m[1][0] << divider << m[1][1] << divider << m[1][2] << divider << m[1][3] << "\n"
            << m[2][0] << divider << m[2][1] << divider << m[2][2] << divider << m[2][3] << "\n"
            << m[3][0] << divider << m[3][1] << divider << m[3][2] << divider << m[3][3] << "\n";
        return out.str();
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

    inline Matrix<T>& operator *= (const Matrix<T> &other) {
        *this = *this * other;
        return *this;
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

    inline Matrix<T>& operator *= (T other) {
        *this = *this * other;
        return *this;
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

    inline friend std::ostream& operator << (std::ostream &os, const Matrix &matrix) {
        os << matrix.to_string();
        return os;
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
