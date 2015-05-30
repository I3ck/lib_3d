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
};

} // namespace lib_3d


#endif // POINTCLOUD_H_INCLUDED
