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
#include "Vec.h"
#include "Matrix.h"

namespace lib_3d {

template <typename T>
class MatrixFactory {
public:

//------------------------------------------------------------------------------

    static Matrix<T> zeroes() {
        return Matrix<T>(
            {{ {0.0, 0.0, 0.0, 0.0}
             , {0.0, 0.0, 0.0, 0.0}
             , {0.0, 0.0, 0.0, 0.0}
             , {0.0, 0.0, 0.0, 0.0}
            }});
    }

//------------------------------------------------------------------------------

    static Matrix<T> unity() {
        return Matrix<T>(
            {{ {1.0, 0.0, 0.0, 0.0}
             , {0.0, 1.0, 0.0, 0.0}
             , {0.0, 0.0, 1.0, 0.0}
             , {0.0, 0.0, 0.0, 1.0}
            }});
    }

//------------------------------------------------------------------------------

    static Matrix<T> translation(T x, T y, T z) {
        return Matrix<T>(
            {{ {1.0, 0.0, 0.0, x  }
             , {0.0, 1.0, 0.0, y  }
             , {0.0, 0.0, 1.0, z  }
             , {0.0, 0.0, 0.0, 1.0}
            }});
    }

    static Matrix<T> translation(Vec<T> data) {
        return translation(data.x, data.y, data.z);
    }

//------------------------------------------------------------------------------

    static Matrix<T> scaling(T x, T y, T z) {
        return Matrix<T>(
            {{ {x,   0.0, 0.0, 0.0}
             , {0.0, y,   0.0, 0.0}
             , {0.0, 0.0, z,   0.0}
             , {0.0, 0.0, 0.0, 1.0}
            }});
    }

    static Matrix<T> scaling(Vec<T> data) {
        return scaling(data.x, data.y, data.z);
    }

    static Matrix<T> scaling(T scale) {
        return scaling(scale, scale, scale);
    }

//------------------------------------------------------------------------------

    static Matrix<T> rotation(T degX, T degY, T degZ) {
        T x = LIB_3D_DEG_TO_RAD * degX;
        T y = LIB_3D_DEG_TO_RAD * degY;
        T z = LIB_3D_DEG_TO_RAD * degZ;

        auto rotX = Matrix<T>(
            {{ {1.0, 0.0,    0.0,     0.0}
             , {0.0, cos(x), -sin(x), 0.0}
             , {0.0, sin(x),  cos(x), 0.0}
             , {0.0, 0.0,    0.0,     1.0}
            }});

        auto rotY = Matrix<T>(
            {{ { cos(y), 0.0, sin(y), 0.0}
             , {0.0,     1.0, 0.0,    0.0}
             , {-sin(y), 0.0, cos(y), 0.0}
             , {0.0,     0.0, 0.0,    1.0}
            }});

        auto rotZ = Matrix<T>(
            {{ {cos(z), -sin(z), 0.0, 0.0}
             , {sin(z),  cos(z), 0.0, 0.0}
             , {0.0,    0.0,     1.0, 0.0}
             , {0.0,    0.0,     0.0, 1.0}
            }});

        return rotX * rotY * rotZ;
    }

    static Matrix<T> rotation(Vec<T> data) {
        return rotation(data.x, data.y, data.z);
    }

    static Matrix<T> rotation(T deg, Vec<T> const& u) {
        const auto x = LIB_3D_DEG_TO_RAD * deg;

        return Matrix<T>(
            {{ {cos(x) + u.x*u.x*(1.0 - cos(x)),     u.x*u.y*(1.0 - cos(x)) - u.z*sin(x), u.x*u.z*(1.0 - cos(x)) + u.y*sin(x), 0.0}
             , {u.y*u.x*(1.0 - cos(x)) + u.z*sin(x), cos(x) + u.y*u.y*(1.0 - cos(x)),     u.y*u.z*(1.0 - cos(x)) - u.x*sin(x), 0.0}
             , {u.z*u.x*(1.0 - cos(x)) - u.y*sin(x), u.z*u.y*(1.0 - cos(x)) + u.x*sin(x), cos(x) + u.z*u.z*(1.0 - cos(x)),     0.0}
             , {0.0,                                 0.0,                                 0.0,                                 1.0}
            }});
    }

//------------------------------------------------------------------------------

    static Matrix<T> perspective(T width, T height, T close, T away, T fovDeg) {
        T fov = LIB_3D_DEG_TO_RAD * fovDeg;
        T range = close-away;
        T tanFovHalf = tan(fov/2.0);

        return Matrix<T>(
            {{ {1.0 / (tanFovHalf * away), 0.0,              0.0,                     0.0}
             , {0.0,                       1.0 / tanFovHalf, 0.0,                     0.0}
             , {0.0,                       0.0,              (-close - away) / range, 2.0 * away * close / range}
             , {0.0,                       0.0,              1.0,                     1.0}
            }});
    }

//------------------------------------------------------------------------------

    static Matrix<T> look_at(const Vec<T> &target, const Vec<T> &up) {
        Vec<T> N = target;
        N.normalize();

        Vec<T> U = up;
        U.normalize();

        U = U.cross(target);

        Vec<T> V = N.cross(U);

        return Matrix<T>(
            {{ {U.x, U.y, U.z, 0.0}
             , {V.x, V.y, V.z, 0.0}
             , {N.x, N.y, N.z, 0.0}
             , {0.0, 0.0, 0.0, 1.0}
            }});
    }

//------------------------------------------------------------------------------

};

} // namespace lib_3d


#endif // MATRIX_FACTORY_H_INCLUDED
