/**
 * \file    Point.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <sstream>
#include <cmath>

#include "Matrix.h"

namespace lib_3d {

template <typename T>
class Point {

public:
    T x, y, z, w;

//------------------------------------------------------------------------------

    Point(T x=0.0, T y=0.0, T z=0.0, T w=1.0) :
        x(x),
        y(y),
        z(z),
        w(w)
    {
    }

//------------------------------------------------------------------------------

    ~Point() {

    }

//------------------------------------------------------------------------------

    Point& move_by(T x, T y, T z) {
        this->x+=x;
        this->y+=y;
        this->z+=z;
        return *this;
    }

    Point& move_by(Point other) {
        this->x+=other.x;
        this->y+=other.y;
        this->z+=other.z;
        return *this;
    }

//------------------------------------------------------------------------------

    ///@todo mirroring methods mirror_xy mirror_xz mirror y_z etc...
    ///@todo mirror on point
    ///@todo mirror on panes

    Point& mirror_point(T xValue = 0, T yValue = 0, T zValue = 0) {
        mirror_point(Point{xValue, yValue});
        return *this;
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

    T distance_to(Point other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2) + pow(z-other.z,2)  );
    }

    T distance_to(T x, T y, T z) const {
        return distance_to(Point{x,y,z});
    }

//------------------------------------------------------------------------------

    Point center_between(Point other) const {
        return Point{(T)(x + (other.x - x) / 2.0), (T)(y + (other.y - y) / 2.0), (T)(z + (other.z - z) / 2.0)};
    }


//------------------------------------------------------------------------------

    bool similar_to(Point other, T maxDistance) const {
        if (distance_to(other) > maxDistance)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (Point other) const {
        if (x == other.x && y == other.y && z == other.z && w == other.w)
            return true;
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (Point other) const {
        return equal_to(other);
    }

    bool operator != (Point other) const {
        return !equal_to(other);
    }

    bool operator < (Point other) const {
        return x < other.x ||
               (x == other.x && y < other.y) ||
               (x == other.x && y == other.y && z < other.z) ||
               (x == other.x && y == other.y && z == other.z && w < other.w);
    }

    friend std::ostream &operator << (std::ostream &os, Point point) {
        os << point.to_string();
        return os;
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

//------------------------------------------------------------------------------

    inline Point<T> operator * (const Matrix<T> &m) const {
        Point<T> copy = *this;
        Point<T> out;
        for(size_t i = 0; i < 4; ++i) {
            for(size_t j =0; j < 4; ++j) {
                out[i] += m.m[i][j] * copy[j];
            }
        }
        return out;
    }
};

} // namespace lib_3d


#endif // POINT_H_INCLUDED
