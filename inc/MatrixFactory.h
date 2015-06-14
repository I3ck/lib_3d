/**
 * \file    MatrixFactory.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef MATRIX_FACTORY_H_INCLUDED
#define MATRIX_FACTORY_H_INCLUDED

#include "constants.h"
#include "Matrix.h"

namespace lib_3d {

template <typename T>
class MatrixFactory {
public:

//------------------------------------------------------------------------------

    static Matrix<T> zeroes() {
        Matrix<T> out;
        out.m[0][0] = 0.0;  out.m[0][1] = 0.0;  out.m[0][2] = 0.0; out.m[0][3] = 0.0;
        out.m[1][0] = 0.0;  out.m[1][1] = 0.0;  out.m[1][2] = 0.0; out.m[1][3] = 0.0;
        out.m[2][0] = 0.0;  out.m[2][1] = 0.0;  out.m[2][2] = 0.0; out.m[2][3] = 0.0;
        out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0; out.m[3][3] = 0.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> unity() {
        Matrix<T> out;
        out.m[0][0] = 1.0;  out.m[0][1] = 0.0;  out.m[0][2] = 0.0; out.m[0][3] = 0.0;
        out.m[1][0] = 0.0;  out.m[1][1] = 1.0;  out.m[1][2] = 0.0; out.m[1][3] = 0.0;
        out.m[2][0] = 0.0;  out.m[2][1] = 0.0;  out.m[2][2] = 1.0; out.m[2][3] = 0.0;
        out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0; out.m[3][3] = 1.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> translation(T x, T y, T z) {
        Matrix<T> out;
        out.m[0][0] = 1.0;  out.m[0][1] = 0.0;  out.m[0][2] = 0.0; out.m[0][3] = x;
        out.m[1][0] = 0.0;  out.m[1][1] = 1.0;  out.m[1][2] = 0.0; out.m[1][3] = y;
        out.m[2][0] = 0.0;  out.m[2][1] = 0.0;  out.m[2][2] = 1.0; out.m[2][3] = z;
        out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0; out.m[3][3] = 1.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> scaling(T x, T y, T z) {
        Matrix<T> out;
        out.m[0][0] = x  ;  out.m[0][1] = 0.0;  out.m[0][2] = 0.0;  out.m[0][3] = 0.0;
        out.m[1][0] = 0.0;  out.m[1][1] =   y;  out.m[1][2] = 0.0;  out.m[1][3] = 0.0;
        out.m[2][0] = 0.0;  out.m[2][1] = 0.0;  out.m[2][2] =   z;  out.m[2][3] = 0.0;
        out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0;  out.m[3][3] = 1.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> scaling(T scale) {
        Matrix<T> out;
        out.m[0][0] = scale;  out.m[0][1] = 0.0;    out.m[0][2] = 0.0;    out.m[0][3] = 0.0;
        out.m[1][0] = 0.0;    out.m[1][1] = scale;  out.m[1][2] = 0.0;    out.m[1][3] = 0.0;
        out.m[2][0] = 0.0;    out.m[2][1] = 0.0;    out.m[2][2] = scale;  out.m[2][3] = 0.0;
        out.m[3][0] = 0.0;    out.m[3][1] = 0.0;    out.m[3][2] = 0.0;    out.m[3][3] = 1.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> rotation(T degX, T degY, T degZ) {
        T x = LIB_3D_DEG_TO_RAD * degX;
        T y = LIB_3D_DEG_TO_RAD * degY;
        T z = LIB_3D_DEG_TO_RAD * degZ;

        Matrix<T>
            rotX, rotY, rotZ;

        rotX.m[0][0] = 1.0;     rotX.m[0][1] = 0.0;      rotX.m[0][2] = 0.0;      rotX.m[0][3] = 0.0;
        rotX.m[1][0] = 0.0;     rotX.m[1][1] = cos(x);   rotX.m[1][2] = -sin(x);  rotX.m[1][3] = 0.0;
        rotX.m[2][0] = 0.0;     rotX.m[2][1] = sin(x);   rotX.m[2][2] = cos(x);   rotX.m[2][3] = 0.0;
        rotX.m[3][0] = 0.0;     rotX.m[3][1] = 0.0;      rotX.m[3][2] = 0.0;      rotX.m[3][3] = 1.0;

        rotY.m[0][0] = cos(y);  rotY.m[0][1] = 0.0;      rotY.m[0][2] = sin(y);   rotY.m[0][3] = 0.0;
        rotY.m[1][0] = 0.0;     rotY.m[1][1] = 1.0;      rotY.m[1][2] = 0.0;      rotY.m[1][3] = 0.0;
        rotY.m[2][0] = -sin(y); rotY.m[2][1] = 0.0;      rotY.m[2][2] = cos(y);   rotY.m[2][3] = 0.0;
        rotY.m[3][0] = 0.0;     rotY.m[3][1] = 0.0;      rotY.m[3][2] = 0.0;      rotY.m[3][3] = 1.0;

        rotZ.m[0][0] = cos(z);  rotZ.m[0][1] = -sin(z);  rotZ.m[0][2] = 0.0;      rotZ.m[0][3] = 0.0;
        rotZ.m[1][0] = sin(z);  rotZ.m[1][1] = cos(z);   rotZ.m[1][2] = 0.0;      rotZ.m[1][3] = 0.0;
        rotZ.m[2][0] = 0.0;     rotZ.m[2][1] = 0.0;      rotZ.m[2][2] = 1.0;      rotZ.m[2][3] = 0.0;
        rotZ.m[3][0] = 0.0;     rotZ.m[3][1] = 0.0;      rotZ.m[3][2] = 0.0;      rotZ.m[3][3] = 1.0;

        return rotX * rotY * rotZ;
    }

//------------------------------------------------------------------------------

    static Matrix<T> perspective(T width, T height, T close, T away, T fovDeg) {
        T fov = LIB_3D_DEG_TO_RAD * fovDeg;
        T ratio = width/height;
        T range = close - away;
        T tanFovHalf = tan(fov/2.0);

        Matrix<T> out;
        out.m[0][0] = 1.0 / (tanFovHalf * away);  out.m[0][1] = 0.0;               out.m[0][2] = 0.0;                      out.m[0][3] = 0.0;
        out.m[1][0] = 0.0;                        out.m[1][1] = 1.0 / tanFovHalf;  out.m[1][2] = 0.0;                      out.m[1][3] = 0.0;
        out.m[2][0] = 0.0;                        out.m[2][1] = 0.0;               out.m[2][2] = (-close - away) / range;  out.m[2][3] = 2.0 * away * close / range;
        out.m[3][0] = 0.0;                        out.m[3][1] = 0.0;               out.m[3][2] = 1.0;                      out.m[3][3] = 0.0;
        return out;
    }

//------------------------------------------------------------------------------

    static Matrix<T> look_at(const Vec<T> &target, const Vec<T> &up) {
      Vec<T> N = target;
      N.normalize();

      Vec<T> U = up;
      U.normalize();

      U = U.cross(target);

      Vec<T> V = N.cross(U);

      Matrix<T> out;
      out.m[0][0] = U.x;  out.m[0][1] = U.y;  out.m[0][2] = U.z;  out.m[0][3] = 0.0f;
      out.m[1][0] = V.x;  out.m[1][1] = V.y;  out.m[1][2] = V.z;  out.m[1][3] = 0.0f;
      out.m[2][0] = N.x;  out.m[2][1] = N.y;  out.m[2][2] = N.z;  out.m[2][3] = 0.0f;
      out.m[3][0] = 0.0;  out.m[3][1] = 0.0;  out.m[3][2] = 0.0;  out.m[3][3] = 1.0f;      

      return out;
    }

//------------------------------------------------------------------------------

};

} // namespace lib_3d


#endif // MATRIX_FACTORY_H_INCLUDED
