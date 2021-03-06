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

#ifndef POINTCLOUD_H_INCLUDED
#define POINTCLOUD_H_INCLUDED

#include "Vec.h"

#include <vector>
#include <fstream>
#include <algorithm>

namespace lib_3d {

template <typename POINTTYPE>
class PointCloud {
private:
    typedef typename POINTTYPE::value_type T;

protected:
    std::vector<POINTTYPE> points;

public:

    PointCloud() {
    }

    PointCloud(size_t nPoints) {
        points.reserve(nPoints);
    }

    PointCloud(std::vector<POINTTYPE> const& points) :
        points(points)
    {}

    PointCloud(std::vector<POINTTYPE> && points) :
        points(std::move(points))
    {}

    template<class InputIterator>
    PointCloud(InputIterator first, InputIterator last) {
        while(first != last) {
            points.push_back(*first);
            ++first;
        }
    }

//------------------------------------------------------------------------------

    ~PointCloud() {

    }

//------------------------------------------------------------------------------

    std::vector<POINTTYPE> get_points() const {
        return points;
    }

    std::vector<T> get_coordinates() const {
      std::vector<T> out;
      for(const auto& point : points) {
        out.push_back(point.x);
        out.push_back(point.y);
        out.push_back(point.z);
      }
      return out;
    }

//------------------------------------------------------------------------------

    size_t number_points() const {
        return points.size();
    }

//------------------------------------------------------------------------------

    ///@todo mirroring

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
        std::string output("");

        for(const auto &p : points)
            output += p.to_string(divider) + "\n";

        return output;
    }

//------------------------------------------------------------------------------

    bool to_file(const std::string &path) const {
         std::ofstream out(path.c_str());
         if(!out.good())
             return false;
         out << to_string() << "\n";
         out.close();
         return true;
     }

//------------------------------------------------------------------------------

    ///@todo from file from string

//------------------------------------------------------------------------------

    PointCloud& push_back(POINTTYPE const& point) {
        points.push_back(point);
        return *this;
    }

    PointCloud& push_back(POINTTYPE && point) {
        points.push_back(std::move(point));
        return *this;
    }

    PointCloud& push_back(T x, T y, T z) {
        push_back(POINTTYPE{x, y, z});
        return *this;
    }

    PointCloud& push_back(const PointCloud &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            push_back(*i);
        return *this;
    }

    PointCloud& emplace_back(T x, T y, T z) {
        points.emplace_back(x, y, z);
        return *this;
    }


//------------------------------------------------------------------------------

    PointCloud& pop_back() {
        points.pop_back();
        return *this;
    }

//------------------------------------------------------------------------------

    size_t size() const {
        return points.size();
    }

//------------------------------------------------------------------------------

    T length() const {
        if(size() < 2)
            return 0;
        T l(0);

        for(auto i = points.cbegin()+1; i != points.cend(); ++i)
            l += i->distance_to(*(i-1));

        return l;
    }

//------------------------------------------------------------------------------

    POINTTYPE first() const {
        return points[0];
    }

    POINTTYPE last() const {
        return points[size()-1];
    }

//------------------------------------------------------------------------------

    PointCloud& reserve(size_t i) {
        points.reserve(i);
        return *this;
    }

//------------------------------------------------------------------------------

    PointCloud& clear() {
        points.clear();
        return *this;
    }

//------------------------------------------------------------------------------

    PointCloud& reverse() {
        std::reverse(points.begin(), points.end());
        return *this;
    }

//------------------------------------------------------------------------------

    POINTTYPE center() const {
        T
            sumX(0.0),
            sumY(0.0),
            sumZ(0.0);

        for(const auto &i : points) {
            sumX += i.x;
            sumY += i.y;
            sumZ += i.z;
        }

        return POINTTYPE{sumX / size(), sumY / size(), sumZ / size()};
    }

//------------------------------------------------------------------------------

    bool similar_to(const PointCloud &other, T maxDistance) const {
        if(size() != other.size())
            return false;
        for(unsigned int i = 0; i < size(); ++i) {
            if(!points[i].similar_to(other[i], maxDistance))
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (const PointCloud &other) const {
        if(size() != other.size())
            return false;
        for(unsigned int i = 0; i < size(); ++i) {
            if(!points[i].equal_to(other[i]))
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    PointCloud& range(unsigned int indexStart, unsigned int indexEnd) {
        if(indexStart > indexEnd)
            return *this;

        if(indexStart >= size() || indexEnd >= size())
            return *this;

        if(indexStart == 0 && indexEnd == size()-1)
            return *this;

        PointCloud<POINTTYPE> tmp;

        for(unsigned int i = indexStart; i <= indexEnd; ++i) {
            tmp += (*this)[i];
        }

        *this = tmp;
        return *this;
    }

//------------------------------------------------------------------------------

    typename std::vector <POINTTYPE >::iterator begin() {
        return points.begin();
    }

    typename std::vector <POINTTYPE >::iterator end() {
        return points.end();
    }

    typename std::vector <POINTTYPE >::const_iterator cbegin() const {
        return points.cbegin();
    }

    typename std::vector <POINTTYPE >::const_iterator cend() const {
        return points.cend();
    }

    typename std::vector <POINTTYPE >::reverse_iterator rbegin() {
        return points.rbegin();
    }

    typename std::vector <POINTTYPE >::reverse_iterator rend() {
        return points.rend();
    }

//------------------------------------------------------------------------------

    bool operator == (const PointCloud &other) const {
        return equal_to(other);
    }

    bool operator != (const PointCloud &other) const {
        return !equal_to(other);
    }

    PointCloud<POINTTYPE>& operator += (const PointCloud<POINTTYPE> &other) {
        push_back(other);
        return *this;
    }

    PointCloud<POINTTYPE>& operator += (POINTTYPE other) {
        push_back(other);
        return *this;
    }

    PointCloud<POINTTYPE> operator + (const PointCloud<POINTTYPE> &other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    PointCloud<POINTTYPE> operator + (POINTTYPE other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    POINTTYPE operator [] (unsigned int i) const {
        return points[i];
    }

    POINTTYPE& operator [] (unsigned int i) {
        return points[i];
    }

    operator std::vector < Vec <T> > () const { ///@todo overload to return constref or even move
        return points;
    }

//------------------------------------------------------------------------------

    friend std::ostream &operator << (std::ostream &os, const PointCloud &path) {
        os << path.to_string();
        return os;
    }

    inline PointCloud<POINTTYPE> operator * (const Matrix<T> &m) const {
        PointCloud<POINTTYPE> out;
        for(const auto& point : points)
            out.points.push_back( point * m ); ///@todo use "add or proper method once implemented"
        return out;
    }

    inline PointCloud<POINTTYPE> operator *= (const Matrix<T> &m) {
        for(auto& point : points)
            point *= m;
        return *this;
    }
};

} // namespace lib_3d


#endif // POINTCLOUD_H_INCLUDED
