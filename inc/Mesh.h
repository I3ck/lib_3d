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
#include <unordered_map>

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
              uint32_t nFacets;
              uint16_t attributes;

              in.read((char*)&header, sizeof(header));
              in.read((char*)&nFacets, sizeof(nFacets));

#pragma pack(push, 1)
              struct Data
              {
              public:
                  float
                    normalX,
                    normalY,
                    normalZ,
                    pos1X,
                    pos1Y,
                    pos1Z,
                    pos2X,
                    pos2Y,
                    pos2Z,
                    pos3X,
                    pos3Y,
                    pos3Z;
                  uint16_t attributes;
              };
#pragma pack(pop)

              auto datas = std::vector<Data>();
              datas.resize(nFacets);
              in.read((char*)datas.data(), datas.size() * sizeof(Data));
              in.close();

              dupedPoints.reserve(3*datas.size());
              for (auto const& data : datas)
              {
                  dupedPoints.emplace_back(data.pos1X, data.pos1Y, data.pos1Z);
                  dupedPoints.emplace_back(data.pos2X, data.pos2Y, data.pos2Z);
                  dupedPoints.emplace_back(data.pos3X, data.pos3Y, data.pos3Z);
              }
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
                      dupedPoints.push_back(pA);
                      dupedPoints.push_back(pB);
                      dupedPoints.push_back(pC);

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
          
          std::unordered_map<POINTTYPE, std::vector<size_t>, PointHasher<T>> indexedPointMap;
          for (size_t i = 0; i < dupedPoints.size(); ++i)
              indexedPointMap[std::move(dupedPoints[i])].push_back(i);

          std::vector<POINTTYPE> uniquePoints;
          uniquePoints.reserve(indexedPointMap.size());

          std::vector<size_t> vertexIds;
          vertexIds.resize(dupedPoints.size());

          for (auto const& indexedPoint : indexedPointMap)
          {
              uniquePoints.push_back(indexedPoint.first);
              for (auto i : indexedPoint.second)
                  vertexIds[i] = uniquePoints.size()-1;
          }

          for (size_t i = 0; i < vertexIds.size(); i += 3)
            facets.emplace_back(vertexIds[i+0], vertexIds[i+1], vertexIds[i+2]);

          this->points.insert(this->points.end(), std::make_move_iterator(uniquePoints.begin()), std::make_move_iterator(uniquePoints.end())); 

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
