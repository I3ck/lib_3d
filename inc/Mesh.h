/**
 * \file    Facet.h
 * \author  Martin Buck
 * \date    June 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Point.h"
#include "Facet.h"
#include "PointCloud.h"

#include <vector>
#include <sstream>
#include <fstream>

namespace lib_3d {

template <typename POINTTYPE>
class Mesh : public PointCloud<POINTTYPE>{
private:
    typedef typename POINTTYPE::value_type T;
    std::vector<Facet> facets;

public:

    Mesh() :
        PointCloud<POINTTYPE>()
    {
    }

    Mesh(size_t nPoints) :
        PointCloud<POINTTYPE>(nPoints)
    {
        facets.reserve(3 * nPoints); ///@todo 3 times nPoints is worst case, dose this make sense?
    }

//------------------------------------------------------------------------------

    ~Mesh() {

    }

//------------------------------------------------------------------------------

    ///@todo either trim all whitespace at beginning of line or only check for contains instead of begins with
    ///@todo throw away facet normal data
    ///@todo each loop, create 3 facets, fill them with data and only then append to data
    ///@todo reset all containers before loading (or add a boolean to check this)

    bool load_stl(std::string path, bool binary = false) {
        if(binary) {
            ///@todo
        }
        else {
            std::string line;
            std::string identifier;
            std::ifstream in(path.c_str());
            while(std::getline(in,line)) {
                std::stringstream ssLine(line);
                ssLine >> identifier;
                if(identifier == "solid") {
                    std::string name("");
                    std::getline(ssLine, name);
                    std::cout << "SOLID" << name << std::endl;
                }

                else if(identifier == "facet normal") {
                    std::cout << "FACET NORMAL" << std::endl;
                }

                else if(identifier == "outer loop") {
                    std::cout << "OUTER LOOP" << std::endl;
                }

                else if(identifier == "vertex") {
                    std::cout << "VERTEX" << std::endl;
                }

                else if(identifier == "endloop") {
                    std::cout << "END LOOP" << std::endl;
                }

                else if(identifier == "endfacet") {
                    std::cout << "END FACET" << std::endl;
                }

                else if(identifier == "endsolid") {
                    std::cout << "END SOLID" << std::endl;
                }
            }
        }
    }

    bool save_stl(std::string path, bool binary = false) {
        if(binary) {
            ///@todo
        }
        else {
            ///@todo
        }
    }
};

} // namespace lib_3d


#endif // MESH_H_INCLUDED
