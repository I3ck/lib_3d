/**
 * \file    PointVec.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef POINTVEC_H_INCLUDED
#define POINTVEC_H_INCLUDED

#include <sstream>
#include <cmath>

#include "Matrix.h"

namespace lib_3d {

template <typename T>
class PointVec {

public:
    T x, y, z, w;

//------------------------------------------------------------------------------

protected:

    PointVec(T x=0.0, T y=0.0, T z=0.0) :
        x(x),
        y(y),
        z(z),
        w(1.0)
    {
    }

//------------------------------------------------------------------------------

public:

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


    PointVec& move_by(T x, T y, T z) {
        this->x+=x;
        this->y+=y;
        this->z+=z;
        return *this;
    }

    PointVec& move_by(PointVec other) {
        this->x+=other.x;
        this->y+=other.y;
        this->z+=other.z;
        return *this;
    }

//------------------------------------------------------------------------------

    ///@todo mirroring methods mirror_xy mirror_xz mirror y_z etc...
    ///@todo mirror on point
    ///@todo mirror on panes

    PointVec& mirror_point(T xValue = 0, T yValue = 0, T zValue = 0) {
        mirror_point(PointVec{xValue, yValue});
        return *this;
    }


//------------------------------------------------------------------------------
    ///@todo dot
    PointVec cross(PointVec const& other)
    {
        return PointVec( this->y * other.z - this->z * other.y
                       , this->z * other.x - this->x * other.z
                       , this->x * other.y - this->y * other.x);
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

    PointVec& normalize() {
        auto l = abs();
        x /= l;
        y /= l;
        z /= l;
        return *this;
    }

//------------------------------------------------------------------------------

    T distance_to(PointVec other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2) + pow(z-other.z,2)  );
    }

    T distance_to(T x, T y, T z) const {
        return distance_to(PointVec{x,y,z});
    }

//------------------------------------------------------------------------------

    bool similar_to(PointVec other, T maxDistance) const {
        if (distance_to(other) > maxDistance)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (PointVec other) const {
        if (x == other.x && y == other.y && z == other.z && w == other.w)
            return true;
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (PointVec other) const {
        return equal_to(other);
    }

    bool operator != (PointVec other) const {
        return !equal_to(other);
    }

    bool operator < (PointVec other) const {
        return x < other.x ||
               (x == other.x && y < other.y) ||
               (x == other.x && y == other.y && z < other.z);
    }

    PointVec operator * (T x) const {
        auto result = *this;
        result.x *= x;
        result.y *= x;
        result.z *= x;
        return result;
    }

    PointVec operator / (T x) const {
        auto result = *this;
        result.x /= x;
        result.y /= x;
        result.z /= x;
        return result;
    }

    PointVec operator - () const  {
        auto result = *this;
        result.x *= -1.0;
        result.y *= -1.0;
        result.z *= -1.0;
        return result;
    }

    friend std::ostream &operator << (std::ostream &os, PointVec point) {
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
PointVec<T> operator * (T x, PointVec<T> p) {
    p.x *= x;
    p.y *= x;
    p.z *= x;
    return p;
}

template<typename T>
PointVec<T> operator / (T x, PointVec<T> p) {
    p.x /= x;
    p.y /= x;
    p.z /= x;
    return p;
}

} // namespace lib_3d


#endif // POINTVEC_H_INCLUDED
