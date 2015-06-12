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

    size_t number_points() const {
        return this->points.size();
    }

    size_t number_facets() const {
        return facets.size();
    }

//------------------------------------------------------------------------------

    std::vector< Facet > get_facets() const {
        return facets;
    }

    std::vector<size_t> get_ids() const {
        std::vector<size_t> ids;

        for(const auto& i : facets) {
            ids.push_back(i.a);
            ids.push_back(i.b);
            ids.push_back(i.c);
        }

        return ids;
    }

//------------------------------------------------------------------------------

    ///@todo either trim all whitespace at beginning of line or only check for contains instead of begins with
    ///@todo throw away facet normal data
    ///@todo each loop, create 3 facets, fill them with data and only then append to data
    ///@todo reset all containers before loading (or add a boolean to check this)
    ///@todo check for error cases (file not opened, no lines, never reached endloop, no facets or points read ...)

    bool load_stl(std::string path, bool binary = false) {
        facets.clear();
        this->points.clear();

        if(binary) {
            ///@todo
        }
        else {
            bool
                inFacet(false),
                inLoop(false);

            std::string
                line,
                identifier,
                garbage,
                name("");

            std::ifstream in(path.c_str());

            size_t numberTmpPoints(0);

            Point<T> pA, pB, pC;

            while(std::getline(in,line)) {
                std::stringstream ssLine(line);

                ssLine >> identifier;

                if(identifier == "solid") {
                    std::getline(ssLine, name);
                }

                else if(identifier == "facet") {
                    inFacet = true;
                    ssLine >> garbage; //to drop "normal" from "facet normal"
                    ///@todo later add facet information here
                }

                else if(identifier == "outer") {
                    if(inFacet)
                        inLoop = true;
                }

                else if(identifier == "vertex") {
                    if(inLoop) {
                        T a, b, c;

                        ssLine >> a;
                        ssLine >> b;
                        ssLine >> c;

                        switch (numberTmpPoints) {
                            case 0:
                                pA = Point<T>(a, b, c);
                                break;
                            case 1:
                                pB = Point<T>(a, b, c);
                                break;
                            case 2:
                                pC = Point<T>(a, b, c);
                                break;
                        }
                        ++numberTmpPoints;
                    }
                }

                else if(identifier == "endloop") {
                    size_t idA, idB, idC;

                    this->points.push_back(pA);
                    idA = this->points.size() - 1;

                    this->points.push_back(pB);
                    idB = this->points.size() - 1;

                    this->points.push_back(pC);
                    idC = this->points.size() - 1;

                    Facet facet(idA, idB, idC);

                    facets.push_back(facet);

                    numberTmpPoints = 0;
                    inLoop = false;
                }

                else if(identifier == "endfacet") {
                    inFacet = false;
                }

                else if(identifier == "endsolid") {
                    //currently doing nothing
                }
            }
        }

        return true;
    }

    bool save_stl(std::string path, bool binary = false, std::string name = "generated with lib_3d") {
        if(binary) {
            ///@todo
        }
        else {
          ///@todo normals have to be calculated
          std::ofstream out(path.c_str());
          out << "solid " << name << std::endl;
          for(auto facet : facets) {
            Point<T> *pA, *pB, *pC;

            pA = &(this->points[facet.a]);
            pB = &(this->points[facet.b]);
            pC = &(this->points[facet.c]);

            out << "facet normal" << std::endl;
            out << "outer loop" << std::endl;
            out << "vertex " << pA->x << " " << pA->y << " " << pA->z << std::endl;
            out << "vertex " << pB->x << " " << pB->y << " " << pB->z << std::endl;
            out << "vertex " << pC->x << " " << pC->y << " " << pC->z << std::endl;
            out << "endloop" << std::endl;
            out << "endfacet" << std::endl;
          }
          out << "endsolid " << name << std::endl;
          out.close();
        }
    }
};

} // namespace lib_3d


#endif // MESH_H_INCLUDED
