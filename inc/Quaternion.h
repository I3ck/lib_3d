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

#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED


namespace lib_3d {

template <typename T>
class Quaternion {
  

public:

    typedef T value_type;

    T x0, x1, x2, x3; ///@todo make these private / protected?

    Quaternion(T x0=0.0, T x1=0.0, T x2=0.0, T x3=0.0) :
        x0(x0),
        x1(x1),
        x2(x2),
        x3(x3)
    {
    }

//------------------------------------------------------------------------------

    ~Quaternion() {

    }

//------------------------------------------------------------------------------

    void conjugate() {
        x1 = -x1;
        x2 = -x2;
        x3 = -x3;
    }

    Quaternion<T> conjugation() {
        Quaternion<T> out = *this;
        out.x1 = - out.x1;
        out.x2 = - out.x2;
        out.x3 = - out.x3;
        return out;
    }

//------------------------------------------------------------------------------

    T real() const {
        return x0;
    }

//------------------------------------------------------------------------------

    T norm() const {
        return this->dot(*this);
    }

//------------------------------------------------------------------------------

    T abs() const {
        return sqrt(norm());
    }

//------------------------------------------------------------------------------ 

    Quaternion<T> unit() const {
        Quaternion<T> out = *this;
        T norm = norm();
        
        out.x0 /= norm;
        out.x1 /= norm;
        out.x2 /= norm;
        out.x3 /= norm;

        return out;
    }

    ///@todo find proper name
    void unify() const {
        Quaternion<T> tmp = this->unit;
        *this = tmp;
    }

//------------------------------------------------------------------------------


    T dot(Quaternion<T> &other) const {
        return real(  (*this)*conjugation(other)  );
    }

//------------------------------------------------------------------------------

    Quaternion<T> cross(Quaternion<T> &other) const {
        return 0.5 * ((*this) * other - other * (*this));
    }

//------------------------------------------------------------------------------

    inline Quaternion<T> operator + (const Quaternion<T> &other) const {
        return Quaternion<T>{this->x0 + other.x0,
                             this->x1 + other.x1,
                             this->x2 + other.x2,
                             this->x3 + other.x3};
    }

    inline Quaternion<T> operator * (const Quaternion<T> &other) const {
        return Quaternion<T>{this->x0 * other.x0  -  this->x1 * other.x1  -  this->x2 * other.x2  -  this->x3 * other.x3,
                             this->x0 * other.x1  +  this->x1 * other.x0  +  this->x2 * other.x3  -  this->x3 * other.x2,
                             this->x0 * other.x2  -  this->x1 * other.x3  +  this->x2 * other.x0  +  this->x3 * other.x1,
                             this->x0 * other.x3  +  this->x1 * other.x2  -  this->x2 * other.x1  +  this->x3 * other.x0};
    }

    inline friend Quaternion<T> operator * (T other, Quaternion<T> quaternion) {
        Quaternion<T> out = quaternion;
        
        out.x0 *= other;
        out.x1 *= other;
        out.x2 *= other;
        out.x3 *= other;

        return out;
    }
};

} // namespace lib_3d


#endif // QUATERNION_H_INCLUDED
