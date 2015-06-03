/**
 * \file    PointCloud.h
 * \author  Martin Buck
 * \date    May 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef POINTCLOUD_H_INCLUDED
#define POINTCLOUD_H_INCLUDED

#include "Point.h"

#include <vector>

namespace lib_3d {

template <typename T>
class PointCloud {
private:
    std::vector< Point<T> > points;

public:

    PointCloud() {
    }

    PointCloud(size_t nPoints) {
        points.reserve(nPoints);
    }

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

    PointCloud& push_back(Point<T> point) {
        points.push_back(point);
        return *this;
    }

    PointCloud& push_back(T x, T y, T z) {
        push_back(Point<T>{x, y, z});
        return *this;
    }

    PointCloud& push_back(const PointCloud &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            push_back(*i);
        return *this;
    }

    PointCloud& emplace_back(Point<T> point) {
        points.emplace_back(point);
        return *this;
    }

    PointCloud& emplace_back(T x, T y, T z) {
        emplace_back(Point<T>{x, y, z});
        return *this;
    }

    PointCloud& emplace_back(const PointCloud &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            emplace_back(*i);
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

    Point<T> first() const {
        return points[0];
    }

    Point<T> last() const {
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

    Point<T> center() const {
        T
            sumX(0.0),
            sumY(0.0),
            sumZ(0.0);

        for(const auto &i : points) {
            sumX += i.x;
            sumY += i.y;
            sumZ += i.z;
        }

        return Point<T>{sumX / size(), sumY / size(), sumZ / size()};
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

        PointCloud<T> tmp;

        for(unsigned int i = indexStart; i <= indexEnd; ++i) {
            tmp += (*this)[i];
        }

        *this = tmp;
        return *this;
    }

//------------------------------------------------------------------------------

    typename std::vector <Point<T> >::iterator begin() {
        return points.begin();
    }

    typename std::vector <Point<T> >::iterator end() {
        return points.end();
    }

    typename std::vector <Point<T> >::const_iterator cbegin() const {
        return points.cbegin();
    }

    typename std::vector <Point<T> >::const_iterator cend() const {
        return points.cend();
    }

    typename std::vector <Point<T> >::reverse_iterator rbegin() {
        return points.rbegin();
    }

    typename std::vector <Point<T> >::reverse_iterator rend() {
        return points.rend();
    }

//------------------------------------------------------------------------------

    bool operator == (const PointCloud &other) const {
        return equal_to(other);
    }

    bool operator != (const PointCloud &other) const {
        return !equal_to(other);
    }

    PointCloud<T>& operator += (const PointCloud<T> &other) {
        push_back(other);
        return *this;
    }

    PointCloud<T>& operator += (Point<T> other) {
        push_back(other);
        return *this;
    }

    PointCloud<T> operator + (const PointCloud<T> &other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    PointCloud<T> operator + (Point<T> other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    Point<T> operator [] (unsigned int i) const {
        return points[i];
    }

    Point<T>& operator [] (unsigned int i) {
        return points[i];
    }

    operator std::vector < Point <T> > () const {
        return points;
    }

//------------------------------------------------------------------------------

    friend std::ostream &operator << (std::ostream &os, const PointCloud &path) {
        os << path.to_string();
        return os;
    }

    inline PointCloud<T> operator * (const Matrix<T> &m) const {
        PointCloud<T> out;
        for(const auto& point : points)
            out.points.push_back( point * m ); ///@todo use "add or proper method once implemented"
        return out;
    }
};

} // namespace lib_3d


#endif // POINTCLOUD_H_INCLUDED
