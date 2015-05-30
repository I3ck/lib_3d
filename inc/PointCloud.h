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

//------------------------------------------------------------------------------

    ~PointCloud() {

    }

//------------------------------------------------------------------------------

    inline PointCloud<T> operator * (const Matrix<T> &m) const {
        PointCloud<T> out;
        for(const auto& point : points)
            out.points.push_back( point * m ); ///@todo use "add or proper method once implemented"
        return out;
    }
};

} // namespace lib_3d


#endif // POINTCLOUD_H_INCLUDED
