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
#include <iomanip>
#include <set>

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

    size_t number_facets() const {
        return facets.size();
    }

//------------------------------------------------------------------------------

    std::vector< Facet > get_facets() const {
        return facets;
    }

    std::vector< Vec <T> > get_normals() {
      Point<T> *pA, *pB, *pC;
      std::vector< Vec<T> > normals;

      normals.resize(this->points.size());

      for(size_t i = 0; i < facets.size(); ++i) {
        Facet facet = facets[i];
        pA = &(this->points[facet.a]);
        pB = &(this->points[facet.b]);
        pC = &(this->points[facet.c]);

        Vec<T> vAb = *pA - *pB;
        Vec<T> vBc = *pB - *pC;
        Vec<T> normale = vAb.cross(vBc).normalize();

        normals[i] += normale;
      }

      for(auto &normale : normals) {
        normale.normalize();
      }

      return normals;
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

    std::vector<unsigned int> get_ids_uint() const {
        std::vector<unsigned int> ids;

        for(const auto& i : facets) {
            ids.push_back((unsigned int)i.a);
            ids.push_back((unsigned int)i.b);
            ids.push_back((unsigned int)i.c);
        }
        return ids;
    }

//------------------------------------------------------------------------------

    bool load_stl(std::string path, bool binary = false) { ///@todo loading with wrong flag => endless loop
        facets.clear();
        this->points.clear();

        std::vector<POINTTYPE> dupedPoints;

        try {

          if(binary) {
              std::ifstream in(path.c_str(), std::ifstream::binary);
              ///@todo check for filesize etc.


              uint8_t header[80];
              uint32_t nFacets = (uint32_t) facets.size();
              uint16_t attributes;

              in.read((char*)&header, sizeof(header));
              in.read((char*)&nFacets, sizeof(nFacets));

              for(unsigned int i = 0; i < nFacets; ++i) {
                Point<T> pTmp;
                size_t idA, idB, idC;

                float tmpPoint[3];

                //first point is normale, which is unused
                in.read((char*)&tmpPoint, sizeof(tmpPoint));

                //now read the remaining 3 points
                for(unsigned int j = 0; j < 3; ++j) {
                  in.read((char*)&tmpPoint, sizeof(tmpPoint));
                  pTmp.x = (T)tmpPoint[0];
                  pTmp.y = (T)tmpPoint[1];
                  pTmp.z = (T)tmpPoint[2];

                  dupedPoints.push_back(std::move(pTmp));
                }

                idA = this->points.size() - 3;
                idB = this->points.size() - 2;
                idC = this->points.size() - 1;

                //Facet facet(idA, idB, idC);
                //facets.push_back(facet);

                //read the 0 char
                in.read((char*)&attributes, sizeof(attributes));
              }

              in.close();

              ///@todo below can be done in general
              ///@todo reserve
              std::set<POINTTYPE> uniquePoints; ///@todo unordered set faster?
              std::vector<size_t> vertexIds;
              std::vector<typename::decltype(uniquePoints)::iterator> vertexIterators;
              for (size_t i = 0; i < dupedPoints.size(); ++i) ///@todo could be foreach and move iterator
              {
                auto res = uniquePoints.insert(dupedPoints[i]);
                vertexIterators.push_back(res.first);
              }

              for (auto const& iter : vertexIterators)
                vertexIds.push_back(std::distance(uniquePoints.begin(), iter));

              for (size_t i = 0; i < vertexIds.size(); i += 3) ///@todo correct break condition?
                facets.push_back(Facet (vertexIds[i+0], vertexIds[i+1], vertexIds[i+2]));

              this->points.clear(); ///@todo should already be done in the beginning, one of them can be removed then
              this->points.insert(this->points.end(), std::make_move_iterator(uniquePoints.begin()), std::make_move_iterator(uniquePoints.end())); 

              ///@todo this version is now bugged, since some faces seem to be missing
              ///@todo might actually be correct, but the rendering bugged
              ///@todo also implement for ascii version once optimised and correct


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
        ///@todo specify error types
        catch(...) {
          facets.clear();
          this->points.clear();
          return false;
        }
        return false;
    }

    bool save_stl(std::string path, bool binary = false, std::string name = "generated with lib_3d") {
        if(binary) {
            //make name at least 80 chars long
            //@todo name parameter currently ignored
            std::string spaces(name.size(), ' ' );
            name += spaces;

            //header has to be of size UINT8[80]
            //so leave this string at 79 characters (\0 char at end of it makes it size 80)
            uint8_t header[] = "lib_3d binary format                                                           ";
            uint32_t nFacets = (uint32_t) facets.size();

            std::ofstream out(path.c_str(), std::ofstream::binary);
            if(!out)
              return false;

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

              float facetData[12];

              facetData[0] = (float)(normale.x); facetData[1] = (float)(normale.y); facetData[2] = (float)(normale.z);


              facetData[3] = (float)(pA->x); facetData[4] = (float)(pA->y); facetData[5] = (float)(pA->z);

              facetData[6] = (float)(pB->x); facetData[7] = (float)(pB->y); facetData[8] = (float)(pB->z);

              facetData[9] = (float)(pC->x); facetData[10] = (float)(pC->y); facetData[11] = (float)(pC->z);

              uint16_t attributes = 0;

              out.write((char*)&facetData, sizeof(facetData));

              out.write((char*)&attributes, sizeof(attributes));
            }
            out.close();
        }
        else {
          const int PRECISION(6);

          std::ofstream out(path.c_str());
          if(!out)
            return false;

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

//------------------------------------------------------------------------------

    inline Mesh<POINTTYPE> operator * (const Matrix<T> &m) const {
        Mesh<POINTTYPE> copy = *this;
        for(auto& point : copy.points)
            point *= m;
        return copy;
    }

    inline Mesh<POINTTYPE> operator *= (const Matrix<T> &m) {
        for(auto& point : this->points)
            point *= m;
        return *this;
    }

};

} // namespace lib_3d


#endif // MESH_H_INCLUDED
