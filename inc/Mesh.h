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
#include <cstdint>

namespace lib_3d {

///@todo move to external file
struct FacetData {

  float normal[3];
  float p1[3];
  float p2[3];
  float p3[3];
  uint16_t attributes;
} facetData;



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
            ///@todo not working yet
            ///@todo trim name if longer than 80 chars
            //make name at least 80 chars long
            std::string spaces(name.size(), ' ' );
            name += spaces;

            //header has to be of size UINT8[80]
            //so leave this string at 79 characters (\0 char at end of it makes it size 80)
            uint8_t header[] = "lib_3d binary format                                                          a";
            uint32_t nFacets = (uint32_t) facets.size();

            std::ofstream out(path.c_str(), std::ofstream::binary);
            out.write((char*)&header, sizeof(header));
            out.write((char*)&nFacets, sizeof(nFacets));

            for(auto facet : facets) {
              Point<T> *pA, *pB, *pC;

              pA = &(this->points[facet.a]);
              pB = &(this->points[facet.b]);
              pC = &(this->points[facet.c]);

              Vec<T> vAb = *pA - *pB;
              Vec<T> vBc = *pB - *pC;
              Vec<T> normale = vAb.cross(vBc).normalize();

              FacetData facetData;

              facetData.normal[0] = (float)normale.x;
              facetData.normal[1] = (float)normale.y;
              facetData.normal[2] = (float)normale.z;

              facetData.p1[0] = (float)pA->x;
              facetData.p1[1] = (float)pA->y;
              facetData.p1[2] = (float)pA->z;

              facetData.p2[0] = (float)pB->x;
              facetData.p2[1] = (float)pB->y;
              facetData.p2[2] = (float)pB->z;

              facetData.p3[0] = (float)pC->x;
              facetData.p3[1] = (float)pC->y;
              facetData.p3[2] = (float)pC->z;

              facetData.attributes = 0;


              out.write((char*)&facetData, sizeof(facetData));
            }
            out.close();
        }
        else {
          const int PRECISION(6);
          std::ofstream out(path.c_str());
          out << std::setprecision(PRECISION) << std::showpoint << std::fixed;

          out << "solid " << name << std::endl;
          for(auto facet : facets) {
            Point<T> *pA, *pB, *pC;

            pA = &(this->points[facet.a]);
            pB = &(this->points[facet.b]);
            pC = &(this->points[facet.c]);

            Vec<T> vAb = *pA - *pB;
            Vec<T> vBc = *pB - *pC;
            Vec<T> normale = vAb.cross(vBc).normalize();

            out << "facet normal " << normale.x << " " << normale.y << " " << normale.z << std::endl;
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
