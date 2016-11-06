/**
 * \file    Vec.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED

#include <sstream>
#include <cmath>

#include "Matrix.h"

namespace lib_3d {

template <typename T>
class Vec {

public:

    typedef T value_type;

    T x, y, z, w;

//------------------------------------------------------------------------------

    Vec(T x=0.0, T y=0.0, T z=0.0) :
        x(x),
        y(y),
        z(z),
        w(1.0)
    {
    }

//------------------------------------------------------------------------------

    T get_x() const
    {
        return x;
    }

    T get_y() const
    {
        return y;
    }

    T get_z() const
    {
        return z;
    }

    T get_w() const
    {
        return w;
    }


//------------------------------------------------------------------------------


    Vec& move_by(T x, T y, T z) {
        this->x+=x;
        this->y+=y;
        this->z+=z;
        return *this;
    }

    Vec& move_by(Vec other) {
        this->x+=other.x;
        this->y+=other.y;
        this->z+=other.z;
        return *this;
    }

//------------------------------------------------------------------------------

    Vec<T>& normalize() {
        const T length = this->abs();

        if(length == 0.0) { ///@todo maybe throw exception instead
          this->x = 0.0;
          this->y = 0.0;
          this->z = 1.0;
        }
        else {
          this->x /= length;
          this->y /= length;
          this->z /= length;
        }

        return *this;
    }


//------------------------------------------------------------------------------

    ///@todo mirroring methods mirror_xy mirror_xz mirror y_z etc...
    ///@todo mirror on point
    ///@todo mirror on panes

    Vec& mirror_point(T xValue = 0, T yValue = 0, T zValue = 0) {
        mirror_point(Vec{xValue, yValue});
        return *this;
    }


//------------------------------------------------------------------------------

    Vec cross(Vec const& other)
    {
        return Vec( this->y * other.z - this->z * other.y
                       , this->z * other.x - this->x * other.z
                       , this->x * other.y - this->y * other.x);
    }

//------------------------------------------------------------------------------

    T dot(const Vec<T> &other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }


//------------------------------------------------------------------------------

    ///@todo use matrix for rotation

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
        std::stringstream ss;
        ss << x << divider << y << divider << z;
        return ss.str();
    }

    bool from_string(const std::string &input, std::string divider = " ") {
        ///@todo has to be implemented
        return true;
    }

//------------------------------------------------------------------------------

    T abs() const {
        return sqrt(  pow(x,2) + pow(y,2) + pow(z,2));
    }

//------------------------------------------------------------------------------

    T distance_to(Vec other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2) + pow(z-other.z,2)  );
    }

    T distance_to(T x, T y, T z) const {
        return distance_to(Vec{x,y,z});
    }

//------------------------------------------------------------------------------

    Vec center_between(Vec other) const {
        return Vec{(T)(this->x + (other.x - this->x) / 2.0),
                   (T)(this->y + (other.y - this->y) / 2.0),
                   (T)(this->z + (other.z - this->z) / 2.0)};
    }

//------------------------------------------------------------------------------

    bool similar_to(Vec other, T maxDistance) const {
        if (distance_to(other) > maxDistance)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (Vec other) const {
        if (x == other.x && y == other.y && z == other.z && w == other.w)
            return true;
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (Vec other) const {
        return equal_to(other);
    }

    bool operator != (Vec other) const {
        return !equal_to(other);
    }

    bool operator < (Vec other) const {
        return x < other.x ||
               (x == other.x && y < other.y) ||
               (x == other.x && y == other.y && z < other.z);
    }

    inline Vec<T> operator + (const Vec<T> &other) const {
        Vec<T> out;
        for(size_t i = 0; i < 3; ++i)
          out[i] = (*this)[i] + other[i];
        return out;
    }

    inline Vec<T>& operator += (const Vec<T> &other) {
        for(size_t i = 0; i < 3; ++i)
          (*this)[i] += other[i];
        return *this;
    }

    inline Vec<T> operator - (const Vec<T> &other) const {
        Vec<T> out;
        for(size_t i = 0; i < 3; ++i)
          out[i] = (*this)[i] - other[i];
        return out;
    }

    inline Vec<T>& operator -= (const Vec<T> &other) {
        for(size_t i = 0; i < 3; ++i)
          (*this)[i] -= other[i];
        return *this;
    }

    inline Vec<T> operator * (const Matrix<T> &m) const {
        Vec<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * (*this)[j];
            }
        }
        return out;
    }

    inline Vec<T>& operator *= (const Matrix<T> &m) {
        *this = *this * m;
        return *this;
    }

    Vec operator * (T x) const {
        auto result = *this;
        result.x *= x;
        result.y *= x;
        result.z *= x;
        return result;
    }

    Vec operator / (T x) const {
        auto result = *this;
        result.x /= x;
        result.y /= x;
        result.z /= x;
        return result;
    }

    Vec operator - () const  {
        auto result = *this;
        result.x *= -1.0;
        result.y *= -1.0;
        result.z *= -1.0;
        return result;
    }

    friend std::ostream &operator << (std::ostream &os, Vec point) {
        os << point.to_string();
        return os;
    }

    T operator [] (size_t i) const {
        switch(i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
        }
    }

    T& operator [] (size_t i) {
        switch(i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
        }
    }
};

template<typename T>
Vec<T> operator * (T x, Vec<T> p) {
    p.x *= x;
    p.y *= x;
    p.z *= x;
    return p;
}

template<typename T>
Vec<T> operator / (T x, Vec<T> p) {
    p.x /= x;
    p.y /= x;
    p.z /= x;
    return p;
}

template <typename T>
struct VecHasher
{
  std::size_t operator()(const Vec<T>& p) const
  {
    return std::hash<T>()((T)(3.0 * p.x + 7.0 * p.y + 11.0 * p.z));
  }
};

} // namespace lib_3d


#endif // VEC_H_INCLUDED
