/**
 * \file    MatrixPipe.h
 * \author  Martin Buck
 * \date    June 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef MATRIX_PIPE_H_INCLUDED
#define MATRIX_PIPE_H_INCLUDED

#include "constants.h"
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

    void remove_translation() {
        matrixTranslation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_rotation(T degX, T degY, T degZ) {
        matrixRotation = MatrixFactory<T>::rotation(degX, degY, degZ);
    }

    void remove_rotation() {
        matrixRotation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_scale(T x, T y, T z) {
        matrixScale = MatrixFactory<T>::scaling(x, y, z);
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

    void remove_camera_translation() {
        matrixCameraTranslation = MatrixFactory<T>::unity();
    }

//------------------------------------------------------------------------------

    void add_camera_look() {
        ///@todo missing in factory
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