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

#ifndef MATRIX_PIPE_H_INCLUDED
#define MATRIX_PIPE_H_INCLUDED

#include "constants.h"
#include "Vec.h"
#include "Matrix.h"
#include "MatrixFactory.h"

namespace lib_3d {

template <typename T>
class MatrixPipe {
private:
    Matrix<T>
        matrixTranslation,
        matrixRotation,
        matrixScale,
        matrixPerspective,
        matrixCameraTranslation,
        matrixCameraLook;

public:

    MatrixPipe() {
        matrixTranslation
        = matrixRotation
        = matrixScale
        = matrixPerspective
        = matrixCameraTranslation
        = matrixCameraLook
        = MatrixFactory<T>::unity();
    }

    ~MatrixPipe() {

    }

//------------------------------------------------------------------------------

    void add_translation(T x, T y, T z) {
        matrixTranslation = MatrixFactory<T>::translation(x, y, z);
    }

    void add_translation(Vec<T> data) {
        add_translation(data.x, data.y, data.z);
    }

    void remove_translation() {
        matrixTranslation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_rotation(T degX, T degY, T degZ) {
        matrixRotation = MatrixFactory<T>::rotation(degX, degY, degZ);
    }

    void add_rotation(Vec<T> data) {
        add_rotation(data.x, data.y, data.z);
    }

    void add_rotation(T deg, Vec<T> const& u) {
        matrixRotation = MatrixFactory<T>::rotation(deg, u);
    }

    void remove_rotation() {
        matrixRotation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_scale(T x, T y, T z) {
        matrixScale = MatrixFactory<T>::scaling(x, y, z);
    }

    void add_scale(Vec<T> data) {
       add_scale(data.x, data.y, data.z);
    }

    void add_scale(T scale) {
        add_scale(scale, scale, scale);
    }

    void remove_scale() {
        matrixScale = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_perspective(T width, T height, T close, T away, T fovDeg) {
        matrixPerspective = MatrixFactory<T>::perspective(width, height, close, away, fovDeg);
    }

    void remove_perspective() {
        matrixPerspective = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_camera_translation(T x, T y, T z) {
        matrixCameraTranslation = MatrixFactory<T>::translation(-x, -y, -z);
    }

    void add_camera_translation(Vec<T> data) {
        add_camera_translation(data.x, data.y, data.z);
    }

    void remove_camera_translation() {
        matrixCameraTranslation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_camera_look(const Vec<T> &target, const Vec<T> &up) {
        matrixCameraLook = MatrixFactory<T>::look_at(target, up);
    }

    void remove_camera_look() {
        matrixCameraLook = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    Matrix<T> result() const {
        return matrixPerspective * matrixCameraLook * matrixCameraTranslation *
               matrixTranslation * matrixRotation * matrixScale;
    }
};

} // namespace lib_3d


#endif // MATRIX_PIPE_H_INCLUDED
